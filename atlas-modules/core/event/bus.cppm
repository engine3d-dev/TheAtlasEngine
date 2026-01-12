module;


#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <any>

export module atlas.core.event.bus;

import atlas.core.event.listener;

namespace atlas::event {
    /**
     * @brief Event bus that holds the responsibility to reroute events to the
     * subscribers of those particular event.
     */
    class bus {
    public:
        bus() = default;

        template<typename UEvent>
        void create_listener() {
            size_t id = type_id<UEvent>();
            if (m_event_listeners.contains(id)) {
                return;
            }
            m_event_listeners.emplace(id, listener<UEvent>());
        }

        template<typename UEvent, typename UObject, typename UCallback>
        void subscribe(UObject* p_instance, const UCallback& p_callback) {
            size_t id = type_id<UEvent>();

            if (!m_event_listeners.contains(id)) {
                create_listener<UEvent>();
                return;
            }

            std::any_cast<atlas::event::listener<UEvent>&>(
              m_event_listeners.at(id))
              .subscribe(p_instance, p_callback);

            // return listener<UEvent>().subscribe(p_instance, p_callback);
        }

        template<typename UEvent>
        void publish(UEvent& p_event) {
            size_t id = type_id<UEvent>();

            if (!m_event_listeners.contains(id)) {
                return;
            }

            std::any_cast<atlas::event::listener<UEvent>&>(
              m_event_listeners.at(id))
              .notify_all(p_event);
        }

    private:
        template<typename UEvent>
        size_t type_id() {
            return typeid(UEvent).hash_code();
        }

    private:
        std::unordered_map<size_t, std::any> m_event_listeners;
    };
};