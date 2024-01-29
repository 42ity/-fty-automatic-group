/*  ====================================================================================================================
    remove.cpp - Implementation of remove group job

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

#include "remove.h"
#include "lib/storage.h"
#include "lib/mutex.h"

namespace fty::job {

void Remove::run(const commands::remove::In& in, commands::remove::Out& out)
{
    fty::storage::Mutex::Write mx;
    std::lock_guard<fty::storage::Mutex::Write> lock(mx);

    for(const auto& id: in) {
        auto& line = out.append();
        if (auto ret = Storage::remove(id); !ret) {
            line.append(fty::convert<std::string>(id), ret.error());
        } else {
            line.append(fty::convert<std::string>(id), "Ok");
            pack::UInt64 send;
            send = id;
            notify(commands::notify::Deleted, send);
        }
    }
}

}
