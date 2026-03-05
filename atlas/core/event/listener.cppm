module;

#include <functional>
#include <deque>

export module atlas.core.event:listener;

export namespace atlas::event {
    /**
     * @brief Generic event listener of event type UEvent
     *
     * Listener that can have a different representation of a specific type of
     * UEvent that gets specified to this listener
     */
    template<typename UEvent>
    class listener {
        using event = std::function<void(UEvent&)>;

    public:
        listener() = default;

        template<typename UObject, typename UCallback>
        void subscribe(UObject* p_instance, const UCallback& p_callback) {
            m_event_handlers.push_back(
              [p_instance, p_callback](UEvent& p_event) {
                  (p_instance->*p_callback)(p_event);
              });
        }

        // Notify all subscribers to this particular event listener
        void notify_all(UEvent& p_event) {
            for (const auto& callback : m_event_handlers) {
                callback(p_event);
            }
        }

    private:
        std::deque<event> m_event_handlers;
    };


    template<typename UEvent>
    class immediate_listener {
        using event = std::function<void(UEvent&)>;
    public:
        immediate_listener() = default;

        /**
        * @brief Specify what should trigger for this specific event
        * 
        * Example Usage:
        * 
        * ```C++
        * trigger<event::mesh_reload>(this, &scene::on_reload);
        * ```
        * 
        */
        template<typename UObject, typename UCallback>
        void trigger(UObject* p_instance, const UCallback& p_callback) {
            m_events.push_back(
              [p_instance, p_callback](UEvent& p_event) {
                  (p_instance->*p_callback)(p_event);
              });
        }

        /**
        * @brief Signaling when to trigger specific events
        * 
        * You can choose when to signal whether this is happening once, or happening
        * @param p_event is the event to apply the changes to this event
        */
        void signal(UEvent& p_event) {
            for (const auto& callback : m_events) {
                callback(p_event);
            }
        }
        
    private:
        std::deque<event> m_events;
    };
};