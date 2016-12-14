#include "catch.hpp"
#include "phoenix.hpp"

TEST_CASE("functions")
{
    //REQUIRE(Identity(1)() == 1);
    REQUIRE(Identity(arg1)(0, 1) == 0);
    REQUIRE(Identity(arg2)(0, 1) == 1);
    REQUIRE(Add(arg2, arg1)(1, 2) == 3);
    REQUIRE(Sub(arg2, Add(2, arg1)(1))(1, 2) == -1);
    REQUIRE(Eq(1, arg1)(1) == true);
    REQUIRE(Eq(2, 2)(nullptr) == true);
    REQUIRE(Eq(arg2, 2)(2, 1) == false);
}

