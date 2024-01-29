/*  ====================================================================================================================
    srr.cpp - Implementation of SRR group job

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

#include "srr.h"
#include "common/message.h"
#include "srr/reset.h"
#include "srr/restore.h"
#include "srr/save.h"
#include <fty_common_messagebus.h>
#include <fty_srr_dto.h>

namespace fty::job {

void SrrProcess::run()
{
    using namespace dto;
    using namespace dto::srr;

    if (!m_bus) {
        logError("No messagebus instance available");
    }

    dto::srr::SrrQueryProcessor srrProcessor;

    srrProcessor.saveHandler    = std::bind(&job::srr::save, std::placeholders::_1);
    srrProcessor.restoreHandler = std::bind(&job::srr::restore, std::placeholders::_1);
    srrProcessor.resetHandler   = std::bind(&job::srr::reset, std::placeholders::_1);

    groups::Message resp;
    resp.meta.subject = m_in.meta.subject;
    resp.meta.to      = m_in.meta.replyTo;
    resp.meta.replyTo = m_in.meta.to;

    logDebug("Processing SRR request: {}", m_in.meta.subject.value());

    try {
        // Get request
        auto     legacyMsg = m_in.toMessageBus();
        UserData data      = legacyMsg.userData();
        Query    query;

        data >> query;

        messagebus::UserData respData;
        respData << (srrProcessor.processQuery(query));

        const auto subject = m_in.meta.subject;
        const auto replyTo = m_in.meta.replyTo;

        resp.setData(respData);

        resp.meta.status = groups::Message::Status::Ok;

        if (auto ans = m_bus->reply(replyTo, m_in, resp); !ans) {
            throw Error("Sending reply failed");
        }
    } catch (const Error& err) {
        logError("Error: {}", err.what());
        resp.meta.status = groups::Message::Status::Error;
        resp.setData(err.what());
        if (auto res = m_bus->reply(fty::Channel, m_in, resp); !res) {
            logError(res.error());
        }
    }
}

void SrrProcess::operator()()
{
    run();
}

} // namespace fty::job
