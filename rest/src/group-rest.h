/*  ====================================================================================================================
    group-rest.h - Common message api for automatic group

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

#pragma once
#include "common/message.h"

namespace fty::agroup {

static constexpr const char* AgentName = "automatic_group_rest";


inline fty::groups::Message message(const std::string& subj)
{
    fty::groups::Message msg;
    msg.meta.to      = "automatic-group";
    msg.meta.replyTo = AgentName;
    msg.meta.subject = subj;
    msg.meta.from    = AgentName;
    return msg;
}

}
