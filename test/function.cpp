#include "catch.hpp"
#include "phoenix.hpp"

TEST_CASE("functions")
{
    //REQUIRE(Identity(1)() == 1);
    REQUIRE(Identity()(1) == 1);
    //REQUIRE(Identity(arg2)(0, 1) == 1);
}

