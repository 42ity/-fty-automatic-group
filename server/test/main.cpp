#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include "lib/config.h"
#include "db.h"
#include <fty_log.h>
#include <filesystem>

int main(int argc, char* argv[])
{
    const std::string automaticGroupConfigurationFile = "test/conf/agroup.conf";

    if (auto ret = fty::Config::instance().load(automaticGroupConfigurationFile); !ret) {
        std::cerr << ret.error() << std::endl;
        return EXIT_FAILURE;
    }

    ManageFtyLog::setInstanceFtylog(fty::Config::instance().actorName, fty::Config::instance().logger);
    std::filesystem::remove(fty::Config::instance().dbpath.value());

    int result = Catch::Session().run(argc, argv);

    fty::GroupsDB::destroy();

    return result;
}
