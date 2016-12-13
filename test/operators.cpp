#include "catch.hpp"
#include "phoenix.hpp"

TEST_CASE("operators")
{
    REQUIRE((arg1 + 2)(2) == 4);
    REQUIRE((2 + arg1)(2) == 4);
    REQUIRE((arg1 + arg2)(2, 2) == 4);
}

