#include "catch.hpp"
#include "phoenix.hpp"

TEST_CASE("functions")
{
    //REQUIRE(Identity(1)() == 1);
    REQUIRE(Identity(arg1)(0, 1) == 0);
    REQUIRE(Identity(arg2)(0, 1) == 1);
    REQUIRE(Add(arg2, arg1)(1, 2) == 3);
}

