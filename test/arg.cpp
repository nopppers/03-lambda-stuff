#include "catch.hpp"
#include "phoenix.hpp"

TEST_CASE("arg")
{
    REQUIRE(arg1(1, 2, 3) == 1);
    REQUIRE(arg2(1, 2, 3) == 2);
    REQUIRE(arg3(1, 2, 3) == 3);
}

