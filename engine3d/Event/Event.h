#pragma once
#include <engine3d/Core/core.h>

namespace engine3d{
    enum class EventType{
        None=0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    // EventCategories
    // May want to filter out certain events
    // In other words,receiving all events from application into some kind of
    //  receiving events from my application into some event class.
    // Only carying about the keyboard events
    // Example Scenario - If I wanted to log in mouse events, and do we really
    // want to check to see if the event type is pressed, released, or scrolled
    // So, instead we can just say "give me all the mouse events"
    // Hence why we have EventCategoryEnum
    enum EventCategory{
        None = 0,
        EventCategoryApplication =  bit(0),
        EventCategoryInput =        bit(1),
        EventCategoryKeyboard =     bit(2),
        EventCategoryMouse =        bit(3),
        EventCategoryMouseButton =  bit(4)
    };

    class ENGINE_API Event{
        friend class EventDispatcher; //! @note EventDispatcher accessing Event private variables
    public:

        std::string toString() const{
            return str();
        }

        //! @note Checking if the type of category we are searching have been set.
        inline bool InCategory(EventCategory category) {
            return GetCategoryFlags() & category;
        }

        inline bool IsEventHandled() const { return isEventHandled; }

        bool operator|=(bool value) {
            isEventHandled |= value;
            return isEventHandled;
        }

    private:
        //! @note Type of Events that are going to be handled
        //! @note Name of Event being handled
        virtual EventType GetEventType() const = 0;
        // virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;

        //! @note Will be overrided by different types of Event
        //! @note Returns the name as a string (or any other information related to those events)
        virtual std::string str() const{ return "Event Default str()"; }

    private:
        bool isEventHandled = false;
    };

    class EventDispatcher{
    public:
        EventDispatcher(Event& e) : event(e) {}

        //! @note Instead of passing in std::function<bool(T&)>
        //! @note Lets just assume the type being passed in is going to be a lambda
        /**@note Used like
        
        EventDispatcher<WindowResizedEvent>([](WindowResizedEvent& event){
        });

        void onWindowResize(WindowResizedEvent& event){}

        EventDispatcher.Dispatch<WindowResizedEvent>(onWindowResized);
        
        */

       //! @param T is the type of event we accept
       //! @param EventFunction is the function that we are submitting to our event dispatcher
       //! @example dispatcher.Dispatch<MousePressedEvent>([](){});
       //! @example dispatcher.Dispatch<KeyPressedEvent>(bind(this, &Instance::onKeyPressed))
       //! @note Using bind(...) to bind our member function to dispatch based on that given event type
        template<typename T, typename EventFunction>
        bool Dispatch(EventFunction func){
            //! @note Assuming that the type of given by user is an event
            //! @note TODO - We should have this function know whether to infer function is a kind of event or not.
            if(event.GetEventType() == T::GetStaticType()){
                event.isEventHandled = func(*(T *)&event);
                return true;
            }
            return false;
        }

    private:
        Event& event;
    };

};