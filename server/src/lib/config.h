/*  ====================================================================================================================
    config.h - Implementation of dto config file

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
#include <fty/expected.h>
#include <pack/pack.h>

namespace fty {

class Config : public pack::Node
{
public:
    pack::String dbpath    = FIELD("dbpath");
    pack::String logger    = FIELD("logger");
    pack::String actorName = FIELD("actor-name", "automatic-group");

    using pack::Node::Node;
    META(Config, dbpath, logger, actorName);

public:
    static Config& instance();
    Expected<void> load(const std::string& path = "agroup.conf");
    Expected<void> reload();
private:
    std::string m_path;
};

} // namespace fty
