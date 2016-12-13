#include "catch.hpp"
#include "phoenix.hpp"

TEST_CASE("ref")
{
    int i = 3;
    REQUIRE(ref(i)() == 3);
    ref(i)() = 2;
    REQUIRE(ref(i)() == 2);
    REQUIRE(ref(i)(3) == 2);
    REQUIRE(ref(i)(3, 4) == 2);
    REQUIRE(ref(i)(3, 4, 5) == 2);
}
