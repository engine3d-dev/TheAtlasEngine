#include <boost/ut.hpp>
#include <cmath>


boost::ut::suite<"basic_add"> vec3_test = [](){
    using namespace boost::ut;
    int x = 10;
    int y = 30;
    int z = (x + y);

    expect(z == 0);
};