#include <catch2/catch.hpp>
#include "lib/jobs/resolve-list.h"
#include "lib/jobs/create.h"
#include "db.h"

TEST_CASE("Resolve several groups")
{
    try {
        fty::SampleDb db(R"(
            items:
                - type : Datacenter
                  name : datacenter
                  items:
                    - type     : Server
                      name     : srv1
                      ext-name : srv1
                    - type     : Server
                      name     : srv2
                      ext-name : srv2
                - type : Datacenter
                  name : datacenter1
                  items:
                    - type     : Server
                      name     : srv11
                      ext-name : srv11
                    - type     : Server
                      name     : srv21
                      ext-name : srv21
            )");

        // Definition of group 1
        fty::Group group1;
        group1.name          = "MyGroup1";
        group1.rules.groupOp = fty::Group::LogicalOp::And;

        auto& var1  = group1.rules.conditions.append();
        auto& cond1 = var1.reset<fty::Group::Condition>();
        cond1.field = fty::Group::Fields::Name;
        cond1.value = "srv1";
        cond1.op    = fty::Group::ConditionOp::Contains;

        // Definition of group 2
        fty::Group group2;
        group2.name          = "MyGroup2";
        group2.rules.groupOp = fty::Group::LogicalOp::And;

        auto& var2  = group2.rules.conditions.append();
        auto& cond2 = var2.reset<fty::Group::Condition>();
        cond2.field = fty::Group::Fields::Name;
        cond2.value = "srv2";
        cond2.op    = fty::Group::ConditionOp::Contains;

        // Create the groups
        fty::job::Create create;
        fty::commands::create::Out created;
        REQUIRE_NOTHROW(create.run(group1, created));
        REQUIRE_NOTHROW(create.run(group2, created));

        // Resolve group 1 only
        {
            fty::job::ResolveList resolveList;

            fty::commands::resolve::list::In in;
            fty::commands::resolve::list::Out out;

            in.ids.append(1);

            REQUIRE_NOTHROW(resolveList.run(in, out));

            REQUIRE(out.size() == 1);
            CHECK(out[0].id == 1);
            REQUIRE(out[0].assets.size() == 2);
            CHECK(out[0].assets[0].name == "srv1");
            CHECK(out[0].assets[1].name == "srv11");
        }

        // Resolve group 2 only
        {
            fty::job::ResolveList resolveList;

            fty::commands::resolve::list::In in;
            fty::commands::resolve::list::Out out;

            in.ids.append(2);

            REQUIRE_NOTHROW(resolveList.run(in, out));

            REQUIRE(out.size() == 1);
            CHECK(out[0].id == 2);
            REQUIRE(out[0].assets.size() == 2);
            CHECK(out[0].assets[0].name == "srv2");
            CHECK(out[0].assets[1].name == "srv21");
        }

        // Resolve group 1 and group 2
        {
            fty::job::ResolveList resolveList;

            fty::commands::resolve::list::In in;
            fty::commands::resolve::list::Out out;
            in.ids.append(1);
            in.ids.append(2);

            REQUIRE_NOTHROW(resolveList.run(in, out));

            REQUIRE(out.size() == 2);
            CHECK(out[0].id == 1);
            REQUIRE(out[0].assets.size() == 2);
            CHECK(out[0].assets[0].name == "srv1");
            CHECK(out[0].assets[1].name == "srv11");
            CHECK(out[1].id == 2);
            REQUIRE(out[1].assets.size() == 2);
            CHECK(out[1].assets[0].name == "srv2");
            CHECK(out[1].assets[1].name == "srv21");
        }

        // Resolve all Groups
        {
            fty::job::ResolveList resolveList;

            fty::commands::resolve::list::In  in;
            fty::commands::resolve::list::Out out;

            REQUIRE_NOTHROW(resolveList.run(in, out));

            REQUIRE(out.size() == 2);
            CHECK(out[0].id == 1);
            REQUIRE(out[0].assets.size() == 2);
            CHECK(out[0].assets[0].name == "srv1");
            CHECK(out[0].assets[1].name == "srv11");
            CHECK(out[1].id == 2);
            REQUIRE(out[1].assets.size() == 2);
            CHECK(out[1].assets[0].name == "srv2");
            CHECK(out[1].assets[1].name == "srv21");
        }

    } catch (const std::exception& ex) {
        std::cerr << "Exception thrown: " << ex.what() << std::endl;
        FAIL(ex.what());
    }
}
