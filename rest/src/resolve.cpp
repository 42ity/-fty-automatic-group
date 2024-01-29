/*  ====================================================================================================================
    resolve.cpp - Implementation of resolve operation on any group

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

#include "resolve.h"
#include "common/commands.h"
#include "common/message-bus.h"
#include "common/string.h"
#include "group-rest.h"
#include <asset/json.h>
#include <fty/rest/component.h>
#include <fty/string-utils.h>

namespace fty::agroup {

unsigned Resolve::run()
{
    rest::User user(m_request);
    if (auto ret = checkPermissions(user.profile(), m_permissions); !ret) {
        throw rest::Error(ret.error());
    }

    auto        strIdPrt = m_request.queryArg<std::string>("id");
    std::string jsonBody;

    if (!strIdPrt) {
        jsonBody = m_request.body();
        if (jsonBody.empty()) {
            throw rest::errors::RequestParamRequired("id or rules");
        }
    }

    fty::groups::MessageBus bus;
    if (auto res = bus.init(AgentName); !res) {
        throw rest::errors::Internal(res.error());
    }

    fty::groups::Message msg = message(fty::commands::resolve::Subject);

    fty::commands::resolve::In in;
    if (jsonBody.empty()) {
        if (!fty::groups::isNumeric(*strIdPrt)) {
            throw rest::errors::Internal("Not a number");
        }
        in.id = fty::convert<uint16_t>(*strIdPrt);
    } else if (auto ret = pack::yaml::deserialize(jsonBody, in); !ret) {
        throw rest::errors::Internal(ret.error());
    }

    msg.setData(*pack::json::serialize(in));

    auto ret = bus.send(fty::Channel, msg);
    if (!ret) {
        throw rest::errors::Internal(ret.error());
    }

    commands::resolve::Out data;
    auto                   info = pack::json::deserialize(ret->userData[0], data);
    if (!info) {
        throw rest::errors::Internal(info.error());
    }

    std::vector<std::string> out;
    for (const auto& it : data) {
        std::string json = asset::getJsonAsset(fty::convert<uint32_t>(it.id.value()));
        if (json.empty()) {
            throw rest::errors::Internal("Cannot build asset information");
        }
        out.push_back(json);
    }

    m_reply << "[" << fty::implode(out, ", ") << "]";

    return HTTP_OK;
}

} // namespace fty::agroup

registerHandler(fty::agroup::Resolve)
