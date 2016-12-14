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
    REQUIRE(Eq(2, 2)(NULL) == true);
    REQUIRE(Eq(arg2, 2)(2, 1) == false);
    REQUIRE(Eq(Add(arg1, arg2), Add(arg2, arg1))(2, 3) == true);
    REQUIRE(Add(Sub(arg1, 1), Sub(arg2, 3))(5, 6) == 7);
    REQUIRE(MAdd(3, 2, arg1)(1) == 7);
    REQUIRE(Length(arg1)(std::string("test")) == 4);

    int a = 2;
    REQUIRE(AddTo(ref(a), arg1)(1) == 3);
    REQUIRE(a == 3);
}

