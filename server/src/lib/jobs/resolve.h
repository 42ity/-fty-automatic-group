#pragma once
#include "lib/task.h"
#include <fty_common_db_connection.h>
#include <fty_common_db_dbpath.h>

namespace tnt {
class Connection;
}

namespace fty::job {

std::string groupSql(fty::db::Connection& conn, const Group::Rules& group);

class Resolve : public Task<Resolve, commands::resolve::In, commands::resolve::Out>
{
public:
    using Task::Task;
    void run(const commands::resolve::In& groupId, commands::resolve::Out& assetList);
};

} // namespace fty::job
