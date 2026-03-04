module;
#include <random>
#include <limits>

export module atlas.core.scene.uuid;


export namespace atlas {

    class uuid {
    public:
        uuid() {
            std::random_device rd;
            std::mt19937_64 engine(rd());
            std::uniform_int_distribution<uint64_t> distribution(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max());

            m_uuid_value = distribution(engine);
        }

        operator uint64_t() { return m_uuid_value; }

        operator uint64_t() const { return m_uuid_value; }

    private:
        uint64_t m_uuid_value=0;
    };
};