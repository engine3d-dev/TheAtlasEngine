module;

export module atlas.tests;

import :basic_add;
import :entity_component_system;
import :scene;
import :math;

export void
initialize_tests() {
    test_basic_add();
    math_test();
    test_entity_component_system();
    test_scenes();
}