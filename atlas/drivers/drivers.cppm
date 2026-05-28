module;

#include <string>

export module atlas.drivers;

import atlas.core.utilities;
import atlas.window;
import atlas.drivers.graphics_context;
import atlas.drivers.vulkan.window_context;

/**
 * @brief This drivers.cppm will contain API-agnostic implementation that may be
 * widely implemented differently.
 *
 * Such as Window contexts, graphics API-agnostic implementation, renderers,
 * etc.
 */

export namespace atlas {
    /**
     * @brief constructs an atlas::window
     *
     * There should only ever be one window constructed throughout the entire
     * application
     *
     * @param p_settings is the window settings to construct the window with
     *
     * @return shared_ptr<atlas::window>
     */
    // ref<window> initialize_window(ref<graphics_context> p_context,
    //                               const window_params& p_params,
    //                               graphics_api p_api) {
    //     switch (p_api) {
    //         case graphics_api::vulkan:
    //             return create_ref<vulkan::window_context>(p_context, p_params);
    //         default:
    //             return nullptr;
    //     }
    // }
};