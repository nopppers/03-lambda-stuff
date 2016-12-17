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
    REQUIRE(Add(arg1, Length(arg2))(1, std::string("test")) == 5);
    REQUIRE(Substr(arg2, arg3, arg1)(2, std::string("test"), 1) == "es");
    REQUIRE(NotEq(true, arg2)(true, false) == true);

    const char *s = "s";
    REQUIRE(NotEq(Find(std::string("test"), arg1, 0), std::string::npos)(s) == true);
    REQUIRE(Find(std::string("test"), arg1, 0)(s) == 2);

    int a = 2;
    REQUIRE(AddTo(ref(a), arg1)(1) == 3);
    REQUIRE(a == 3);

    std::vector<int> testVec;
    std::vector<int> expected;
    std::vector<int> output;
    std::back_insert_iterator<std::vector<int> > out(output);
    testVec.push_back(1);
    testVec.push_back(2);
    testVec.push_back(3);
    expected.push_back(2);
    expected.push_back(4);
    expected.push_back(6);
    Transform(Begin(arg1ref), End(arg1ref), arg2, val(Mul(arg1, 2)))(testVec, out);
    REQUIRE(output == expected);

    std::string test = "test";
    REQUIRE(Upper(arg1)(test) == "TEST");
    REQUIRE(ReverseVal(arg1, arg2)(test.begin(), test.end()) == "tset");
}

