/*  ====================================================================================================================
    resolve-list.h - Implementation of resolve list group job

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
#include "lib/task.h"

namespace tnt {
class Connection;
}

namespace fty::job {

class ResolveList : public Task<ResolveList, commands::resolve::list::In, commands::resolve::list::Out>
{
public:
    using Task::Task;
    void run(const commands::resolve::list::In& groupIds, commands::resolve::list::Out& assetGroupList);
};

} // namespace fty::job
