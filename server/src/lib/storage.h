/*  =========================================================================
    storage.h - Implementation of storage

    Copyright (C) 2014 - 2020 Eaton

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
    =========================================================================
 */

#pragma once
#include "common/group.h"
#include <fty/expected.h>

namespace fty {

class Storage
{
public:
    static Storage& instance();

public:
    Storage();
    ~Storage();

    static Expected<Group>          byName(const std::string& name);
    static Expected<Group>          byId(uint64_t id);
    static std::vector<std::string> names();
    static std::vector<uint64_t>    ids();

    static Expected<Group> save(const Group& group);
    static Expected<void>  remove(uint64_t id);
    static Expected<void>  removeByName(const std::string& groupName);
    static Expected<void>  clear();

private:
    class Impl;
    std::unique_ptr<Impl> m_impl{nullptr};
};

} // namespace fty
