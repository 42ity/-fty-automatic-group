/*  =========================================================================
    server.h - Implementation of server

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
#include "common/message-bus.h"
#include <fty_common_dto.h>
#include <fty/event.h>
#include <fty/thread-pool.h>

namespace fty {

class Server
{
public:
    [[nodiscard]] Expected<void> run();
    void                         shutdown();
    void                         wait();

    Event<> stop;

private:
    void process(const groups::Message& msg);
    void srrProcess(const groups::Message& msg);
    void doStop();
    void reloadConfig();

private:
    groups::MessageBus m_bus;
    ThreadPool m_pool;

    Slot<> m_stopSlot       = {&Server::doStop, this};
    Slot<> m_loadConfigSlot = {&Server::reloadConfig, this};
};

} // namespace fty
