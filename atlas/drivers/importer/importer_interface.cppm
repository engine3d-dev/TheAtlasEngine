module;

#include <string>
#include <span>

export module atlas.drivers.importer:interface;

namespace atlas {

    class importer {
    public:
        virtual ~importer() = default;

        virtual bool load(const std::string& p_path) = 0;

        virtual bool load() const = 0;

        //! @return the geometry vertices
        virtual std::span<vk::vertex_input> vertices() = 0;

        //! @return the geometry indices
        virtual std::span<uint32_t> indices() = 0;
    };
};