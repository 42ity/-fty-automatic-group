/*  ====================================================================================================================
    read.cpp - Implementation of read operation on any group

    Copyright (C) 2014 - 2024 Eaton

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

#include "read.h"
#include "common/commands.h"
#include "common/message-bus.h"
#include "common/string.h"
#include "group-rest.h"
#include <fty/rest/audit-log.h>
#include <fty/rest/component.h>
#include <fty_common_asset_types.h>

namespace fty::agroup {

unsigned Read::run()
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

    fty::groups::MessageBus bus;
    if (auto res = bus.init(AgentName); !res) {
        throw rest::errors::Internal(res.error());
    }

    fty::groups::Message msg = message(fty::commands::read::Subject);

    fty::commands::read::In in;
    in.id = fty::convert<uint16_t>(*strIdPrt);

    msg.setData(*pack::json::serialize(in));

    auto ret = bus.send(fty::Channel, msg);
    if (!ret) {
        throw rest::errors::Internal(ret.error());
    }

    commands::read::Out out;

    auto info = pack::json::deserialize(ret->userData[0], out);
    if (!info) {
        throw rest::errors::Internal(info.error());
    }

    m_reply << *pack::json::serialize(out, pack::Option::ValueAsString);

    return HTTP_OK;
}

} // namespace fty::agroup

registerHandler(fty::agroup::Read)
