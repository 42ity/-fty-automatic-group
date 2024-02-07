/*  ====================================================================================================================
    commands.h - Definition of dto commands for automatic group

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
#include "group.h"

namespace fty {

static constexpr const char* Channel = "FTY.Q.GROUP.QUERY";
static constexpr const char* Events  = "FTY.T.GROUP.EVENT";

namespace commands::create {
    static constexpr const char* Subject = "CREATE";

    using In  = Group;
    using Out = Group;
} // namespace commands::create

namespace commands::update {
    static constexpr const char* Subject = "UPDATE";

    using In  = Group;
    using Out = Group;
} // namespace commands::update

namespace commands::remove {
    static constexpr const char* Subject = "DELETE";

    using In  = pack::UInt64List;
    using Out = pack::ObjectList<pack::StringMap>;
} // namespace commands::remove

namespace commands::resolve {
    static constexpr const char* Subject = "RESOLVE";

    struct Request : public pack::Node
    {
        pack::UInt64 id = FIELD("id");
        fty::Group::Rules rules = FIELD("rules");

        using pack::Node::Node;
        META(Request, id, rules);
    };

    struct Answer : public pack::Node
    {
        pack::UInt64 id   = FIELD("id");
        pack::String name = FIELD("name");

        using pack::Node::Node;
        META(Answer, id, name);
    };


    using In  = Request;
    using Out = pack::ObjectList<Answer>;
} // namespace commands::resolve

namespace commands::resolve::list {
    static constexpr const char* Subject = "RESOLVE_LIST";

    struct Request : public pack::Node
    {
        pack::UInt64List ids = FIELD("ids");

        using pack::Node::Node;
        META(Request, ids);
    };

    struct Assets : public pack::Node
    {
        pack::UInt64 id   = FIELD("id");
        pack::String name = FIELD("name");

        using pack::Node::Node;
        META(Assets, id, name);
    };

    struct Answer : public pack::Node
    {
        pack::UInt64 id   = FIELD("id");
        pack::ObjectList<Assets> assets = FIELD("assets");

        using pack::Node::Node;
        META(Answer, id, assets);
    };

    using In  = Request;
    using Out = pack::ObjectList<Answer>;
} // namespace commands::resolve::list


namespace commands::list {
    static constexpr const char* Subject = "LIST";

    struct Answer : public pack::Node
    {
        pack::UInt64 id   = FIELD("id");
        pack::String name = FIELD("name");

        using pack::Node::Node;
        META(Answer, id, name);
    };

    using Out = pack::ObjectList<Answer>;
} // namespace commands::list

namespace commands::read {
    static constexpr const char* Subject = "READ";

    struct Request : public pack::Node
    {
        pack::UInt64 id = FIELD("id");

        using pack::Node::Node;
        META(Request, id);
    };

    using In  = Request;
    using Out = Group;
} // namespace commands::read

namespace commands::notify {
    static constexpr const char* Created = "CREATED";
    static constexpr const char* Updated = "UPDATED";
    static constexpr const char* Deleted = "DELETED";
    using Payload                        = pack::UInt64;
} // namespace commands::notify

} // namespace fty
