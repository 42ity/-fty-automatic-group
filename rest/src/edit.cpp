/*  ====================================================================================================================
    edit.cpp - Implementation of edit operation on any group

    Copyright (C) 2024 Eaton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    ====================================================================================================================
*/

#include "edit.h"
#include <fty/rest/component.h>
#include "common/message-bus.h"
#include "common/string.h"
#include "group-rest.h"
#include "common/commands.h"

namespace fty::agroup {

unsigned Edit::run()
{
    rest::User user(m_request);
    if (auto ret = checkPermissions(user.profile(), m_permissions); !ret) {
        throw rest::Error(ret.error());
    }

    auto strIdPrt = m_request.queryArg<std::string>("id");
    if (!strIdPrt) {
        throw rest::errors::RequestParamRequired("id");
    }

    if (!fty::groups::isNumeric(*strIdPrt)) {
        throw rest::errors::Internal("Not a number");
    }

    std::string json = m_request.body();
    if (json.empty()) {
        throw rest::errors::BadInput("Group payload is empty");
    }

    Group group;
    if (auto ret = pack::json::deserialize(json, group); !ret) {
        throw rest::errors::Internal(ret.error());
    }

    group.id = fty::convert<uint64_t>(*strIdPrt);

    fty::groups::MessageBus bus;
    if (auto res = bus.init(AgentName); !res) {
        throw rest::errors::Internal(res.error());
    }

    auto msg = message(commands::update::Subject);
    msg.setData(*pack::json::serialize(group));

    if (auto ret = bus.send(fty::Channel, msg)) {
        commands::update::Out out;
        if (auto info = pack::json::deserialize(ret->userData[0], out)) {
            m_reply << *pack::json::serialize(out, pack::Option::ValueAsString);
            return HTTP_OK;
        } else {
            throw rest::errors::Internal(info.error());
        }
    } else {
        throw rest::errors::Internal(ret.error());
    }

    return HTTP_OK;
}

}

registerHandler(fty::agroup::Edit)
