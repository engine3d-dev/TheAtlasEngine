import atlas;
#include <print>
#include <memory>

int main() {
    auto app = atlas::initialize_application();

    if(app == nullptr) {
        std::println("Initialize App Failed!!!");
    }
    else {
        std::println("App Initialized Successful!!!");
    }
    
    return 0;
}