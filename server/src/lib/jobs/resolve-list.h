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
