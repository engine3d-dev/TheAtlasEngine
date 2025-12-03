#pragma once
#include <exception>

namespace atlas {
    struct exception_block {
        const char* data=nullptr;
    };

    class invalid_access_exception : public std::exception {
    public:
        invalid_access_exception() = default;
        invalid_access_exception(const char* p_data) : m_block(p_data) {}

        //! @return message given when this exception gets triggered
        [[nodiscard]] const char* what() const override { return m_block.data; }

    private:
        exception_block m_block;
    };
};