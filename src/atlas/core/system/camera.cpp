#include <core/system/camera.hpp>
#include <core/scene/components.hpp>

namespace atlas {

    camera_system::camera_system(const float p_aspect_ratio,
                                 const flecs::world& p_registry)
      : m_aspect_ratio(p_aspect_ratio)
      , m_registry(p_registry) {
        m_query =
          m_registry
            .query_builder<projection_view, transform, perspective_camera>()
            .with<tag_redo::editor>()
            .second<projection_view>()
            .build();
    }

    void camera_system::query_camera_entities() {
        m_query.each([&]([[maybe_unused]] flecs::entity p_entity,
                         projection_view& p_pair,
                         transform& p_transform,
                         perspective_camera& p_camera) {
            if (!p_camera.is_active) {
                return;
            }

            p_pair.projection = glm::mat4(1.f);

            p_pair.projection =
              glm::perspective(glm::radians(p_camera.field_of_view),
                               m_aspect_ratio,
                               p_camera.plane.x,
                               p_camera.plane.y);

            p_pair.view = glm::mat4(1.f);
            glm::quat quaternion = glm::quat({ p_transform.quaternion.w,
                                               p_transform.quaternion.x,
                                               p_transform.quaternion.y,
                                               p_transform.quaternion.z });
            p_pair.view = glm::translate(p_pair.view, p_transform.position) *
                          glm::toMat4(quaternion);
            p_pair.view = glm::inverse(p_pair.view);

            // console_log_warn("mat4 = {}", glm::value_ptr(p_pair.view));
        });
    }
};