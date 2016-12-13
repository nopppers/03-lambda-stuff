#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "phoenix.hpp"


TEST_CASE("val")
{
    REQUIRE(val(1)() == 1);
    REQUIRE(val(1)(2) == 1);
    REQUIRE(val(1)(2, 3) == 1);
    REQUIRE(val(1)(2, 3, 4) == 1);
}