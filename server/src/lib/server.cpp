
/*  =========================================================================
    server.cpp - Implementation of server

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

#include "server.h"
#include "common/message-bus.h"
#include "common/srr.h"
#include "config.h"
#include "daemon.h"
#include "jobs/create.h"
#include "jobs/list.h"
#include "jobs/read.h"
#include "jobs/remove.h"
#include "jobs/resolve.h"
#include "jobs/resolve-list.h"
#include "jobs/srr.h"
#include "jobs/update.h"
#include <fty_common_messagebus.h>

namespace fty {

Expected<void> Server::run()
{
    m_stopSlot.connect(Daemon::instance().stopEvent);
    m_loadConfigSlot.connect(Daemon::instance().loadConfigEvent);

    if (auto res = m_bus.init(Config::instance().actorName); !res) {
        return unexpected(res.error());
    }

    if (auto sub = m_bus.subscribe(Channel, &Server::process, this); !sub) {
        return unexpected(sub.error());
    }

    if (auto sub = m_bus.subscribe(common::srr::Channel, &Server::srrProcess, this); !sub) {
        return unexpected(sub.error());
    }

    return {};
}

void Server::wait()
{
    stop.wait();
}

void Server::doStop()
{
    stop();
}

void Server::reloadConfig()
{
    Config::instance().reload();
}

void Server::process(const groups::Message& msg)
{
    logDebug("Automatic group: got message {}", msg.dump());
    if (msg.meta.subject == commands::create::Subject) {
        m_pool.pushWorker<job::Create>(msg, m_bus);
    } else if (msg.meta.subject == commands::update::Subject) {
        m_pool.pushWorker<job::Update>(msg, m_bus);
    } else if (msg.meta.subject == commands::remove::Subject) {
        m_pool.pushWorker<job::Remove>(msg, m_bus);
    } else if (msg.meta.subject == commands::list::Subject) {
        m_pool.pushWorker<job::List>(msg, m_bus);
    } else if (msg.meta.subject == commands::read::Subject) {
        m_pool.pushWorker<job::Read>(msg, m_bus);
    } else if (msg.meta.subject == commands::resolve::Subject) {
        m_pool.pushWorker<job::Resolve>(msg, m_bus);
    } else if (msg.meta.subject == commands::resolve::list::Subject) {
        m_pool.pushWorker<job::ResolveList>(msg, m_bus);
    }

}

void Server::srrProcess(const groups::Message& msg)
{
    logDebug("Automatic group: got SRR request {}", msg.dump());
    m_pool.pushWorker<job::SrrProcess>(msg, m_bus);
}

void Server::shutdown()
{
    stop();
    m_pool.stop();
}

} // namespace fty
