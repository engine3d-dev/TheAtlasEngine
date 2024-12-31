#include <boost/ut.hpp>

#include <boost/ut.hpp>

boost::ut::suite<"engine3d::object"> scene_test = [](){
    using namespace boost::ut;

    "::object"_test = []{
        expect(true);
    };
};
