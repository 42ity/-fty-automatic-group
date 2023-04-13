#include <catch2/catch.hpp>
#include "lib/jobs/list.h"
#include "lib/jobs/create.h"
#include "lib/storage.h"
#include "db.h"

TEST_CASE("List")
{
    REQUIRE(fty::GroupsDB::init());

    {
        // Create a group
        fty::Group created;
        {
            fty::Group group;
            {
                group.name          = "My group";
                group.rules.groupOp = fty::Group::LogicalOp::And;

                auto& var  = group.rules.conditions.append();
                auto& cond = var.reset<fty::Group::Condition>();
                cond.value = "srv";
                cond.field = fty::Group::Fields::Name;
                cond.op    = fty::Group::ConditionOp::Contains;
            }

            fty::job::Create create;
            REQUIRE_NOTHROW(create.run(group, created));
        }
        CHECK(created.hasValue());

        fty::job::List list;
        fty::commands::list::Out out;

        // Initial size should be zero
        CHECK(out.size() == 0);

        REQUIRE_NOTHROW(list.run(out));
        CHECK(out.size() == 1);
        CHECK(out[0].id == created.id);
        CHECK(out[0].name == created.name);
    }

    CHECK(fty::Storage::clear());
}
