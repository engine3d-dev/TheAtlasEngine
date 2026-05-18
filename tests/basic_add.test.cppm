module;
#include <boost/ut.hpp>
export module atlas.tests:basic_add;

export void test_basic_add() {
    boost::ut::suite<"basic_add"> add_test = []() {
        using namespace boost::ut;

        "unit1"_test = []() {
            int x = 10;
            int y = 30;
            int z = (x + y);

            expect(z == 40);
        };
    };
}