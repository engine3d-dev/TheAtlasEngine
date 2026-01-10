module;

#include <spdlog/common.h>
#include <memory>

export module core:common;


export namespace atlas {
    /**
     * @brief alias to atlas::memory::strong_ptr<T>
     */
    // template<typename T>
    // using strong_ref = memory::strong_ptr<T>;

    // /**
    //  * @brief construct strong_ptr<T> through
    //  * atlas::memory::create_strong_ptr<T>(...);
    //  *
    //  * @tparam ...Args is a template pack for packing in parameters
    //  * @tparam T is the type of object to construct strong_ptr with
    //  * @param p_allocator is a polymorphic allocator when creating strong_ptr's.
    //  * @param ...Args is an argument pack for packing to deduce the types that
    //  * is specified by object of type T
    //  */
    // template<typename T, typename... Args>
    // strong_ref<T> create_strong_ref(
    // std::pmr::polymorphic_allocator<> p_allocator,
    // Args&&... args) {
    //     return memory::make_strong_ptr<T>(p_allocator,
    //                                     std::forward<Args>(args)...);
    // }

    /**
     * @brief alias to memory::optional_ptr<T>
     */
    // template<typename T>
    // using optional_ref = memory::optional_ptr<T>;

    /**
     * @brief alias to std::shared_ptr<T>
     */
    template<typename T>
    using ref = std::shared_ptr<T>;

    /**
     * @brief construct std::make_shared<T>(...);
     *
     * @tparam ...Args is a template pack for packing in parameters
     * @tparam T is the type of object to construct strong_ptr with
     * @param ...Args is an argument pack for packing to deduce the types that
     * is specified by object of type T
     */
    template<typename T, typename... Args>
    constexpr ref<T> create_ref(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    /**
     * @brief alias to std::unique_ptr<T>
     */
    template<typename T>
    using scope = std::unique_ptr<T>;

    /**
     * @brief construct std::make_unique<T>(...);
     *
     * @tparam ...Args is a template pack for packing in parameters
     * @tparam T is the type of object to construct strong_ptr with
     * @param ...Args is an argument pack for packing to deduce the types that
     * is specified by object of type T
     */
    template<typename T, typename... Args>
    constexpr scope<T> create_scope(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
};