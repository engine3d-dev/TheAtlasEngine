#pragma once
#include <memory_resource>

namespace atlas {
    class scene_resources : public std::pmr::memory_resource {
    public:
        scene_resources(char* p_buffer, size_t p_size/*NOLINT*/, size_t p_offset) : m_buffer(p_buffer), m_size(p_size), m_offset(p_offset) {

        }

        [[nodiscard]] bool do_is_equal(const memory_resource& other) const noexcept override {
            // Check if the other resource is the same instance
            return this == &other;
        }

    protected:
        void* do_allocate(size_t p_bytes/*NOLINT*/, [[maybe_unused]] size_t p_alignment) override {
            if(m_offset + p_bytes > m_size) {
                throw std::bad_alloc();
            }

            void* ptr = m_buffer+m_offset;
            m_offset += p_bytes;

            return ptr;
        }

        void do_deallocate([[maybe_unused]] void* p_ptr /*NOLINT*/, [[maybe_unused]] size_t p_bytes, [[maybe_unused]] size_t p_alignment) override {
        }

        

    private:
        char* m_buffer=nullptr;
        size_t m_size=0;
        size_t m_offset=0;
    };
};