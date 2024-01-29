/*  =========================================================================
    mutex.h - Linux mutex simple implementation

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
#include <atomic>
#include <condition_variable>
#include <fty/expected.h>
#include <mutex>
#include <optional>

namespace fty::storage {

class Mutex
{
public:
    enum class AccessType
    {
        READ,
        WRITE
    };

    class Write
    {
        std::unique_ptr<Mutex> m_m;
        bool                   m_locked = false;

    public:
        Write();
        void lock();
        void unlock();
    };

    class Read
    {
        std::unique_ptr<Mutex> m_m;
        bool                   m_locked = false;

    public:
        Read();
        void lock();
        void unlock();
    };

private:
    static std::optional<AccessType> m_currentAccess;
    static std::atomic<uint16_t>     m_activeReaders;
    static std::mutex                m_mxCurrentAccess;
    static std::condition_variable   m_cvAccess;

    int m_fd = 0;

    Expected<void> lock(AccessType access);
    Expected<void> unlock(AccessType access);
};

} // namespace fty::storage
