/*  ====================================================================================================================
    update.cpp - Implementation of update group job

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

#include "update.h"
#include "lib/storage.h"
#include "lib/mutex.h"

namespace fty::job {

void Update::run(const commands::update::In& in, commands::update::Out& out)
{
    if (auto ret = in.check(); !ret) {
        throw Error(ret.error());
    }

    auto found = Storage::byName(in.name);
    if (found && found->id != in.id) {
        throw Error("Another group with name '{}' already exists", in.name.value());
    }
    
    fty::storage::Mutex::Write mx;
    std::lock_guard<fty::storage::Mutex::Write> lock(mx);

    if (auto ret = Storage::save(in); !ret) {
        throw Error(ret.error());
    } else {
        out = *ret;
        notify(commands::notify::Updated, ret->id);
    }
}

}
