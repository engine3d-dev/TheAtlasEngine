#pragma once

namespace atlas {
    struct exception_block {
        const char* data=nullptr;
    };

    class object_accessor_exception {
    public:
        object_accessor_exception(const char* p_data) : m_data(p_data) {}

        [[nodiscard]] const char* what() const { return m_data.data; }

    private:
        exception_block m_data;
    };
};