#pragma once
#include <engine3d/Core/core.h>
#include <engine3d/Graphics/VertexArray.h>

namespace engine3d{

    class Renderer{
    public:

        //! @note If user does not have a valid indices passed into vao object
        //! @note You need to supply either supply a default indices count or use the default 36
        //! @note TODO ---- Move our render pass (actual draw call) to our actual render pass
        //! @note Renderer should be abstracting our API-related draw calls.
        static void RenderPass(Ref<VertexArray>& vao, size_t count=36);

    private:
    };
};