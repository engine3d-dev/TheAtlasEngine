#pragma once
#include <string>
#include <functional>

namespace atlas::ui {
    struct block {
        const char* data = nullptr;
    };

    class menu_bar_exception {
    public:
        menu_bar_exception(const char* p_data)
          : m_data(p_data) {}

        [[nodiscard]] const char* what() const { return m_data.data; }

    private:
        block m_data;
    };
    /**
     * @brief UI wrapper around setting up a menu item group
     *
     *
     * ```C++
     *
     * atlas::ui::menu_item menu;
     *
     * menu.begin();
     *
     * menu.add_child("Save", [](){
     *  // do some operation related to saving
     *  // perhaps saving the scene...
     * });
     *
     * menu.end();
     *
     * ```
     */
    class menu_item {
    public:
        menu_item() = default;

        void begin();

        void end();

        void add_child(const std::string& p_name,
                       const std::function<void()>& p_callback);
    };
};