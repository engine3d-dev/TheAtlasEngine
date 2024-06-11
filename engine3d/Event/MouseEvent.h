// #pragma once
// #include <engine3d/Event/Event.h>
// #include <engine3d/Event/InputPoll.h>

// namespace engine3d{
//     // MouseMovedEvent
//     // (x,y) - is the location of the mouse
//     class ENGINE_API MouseMovedEvent : public Event {
//     public:
//         MouseMovedEvent(float x, float y) : _mouseX(x), _mouseY(y) {}

//         inline float GetX() const { return _mouseX; }
//         inline float GetY() const { return _mouseY; }

//         std::string toString() const override {
//             // return formatCpp::format("MouseMovedEvent: ({}, {})", _mouseX, _mouseY);
//             std::stringstream ss;
//             ss << fmt::format("MouseMovedEvent: ({}, {})", _mouseX, _mouseY);
//             return ss.str();
//         }

//         // In runtime we want to see what event type this is.
//         static EventType GetStaticType() { return EventType::MouseMoved; }

//         // We need another instance of it, so we know what the actual event type it is.
//         virtual EventType GetEventType() const override { return GetStaticType(); }
//         virtual const char* GetName() const override { return "EventType::MouseMoved"; }

//         int GetCategoryFlags() const override{
//             return EventCategoryMouse | EventCategoryInput;
//         }

//     private:
//         float _mouseX;
//         float _mouseY;
//     };

//     // Same thing as MouseMovedEvent, instead we take the offset of x and y.
//     // offset being where we scrolled
//     // Handling x and y because there is horizontal scrolling that we can do.
//     class ENGINE_API MouseScrolledEvent : public Event{
//     public:
//         MouseScrolledEvent(float xOffset, float yOffset) : _xOffset(xOffset), _yOffset(yOffset) {}

//         inline float GetXOffset() const { return _xOffset; }
//         inline float GetYOffset() const { return _yOffset; }

//         std::string toString() const override {
//             // return formatCpp::format("MouseScrolledEvent: ({}, {})", _xOffset, _yOffset);
//             std::stringstream ss;
//             ss << fmt::format("MouseScrolledEvent: ({}, {})", _xOffset, _yOffset);
//             return ss.str();
//         }

//         // In runtime we want to see what event type this is.
//         static EventType GetStaticType() { return EventType::MouseScrolled; }

//         // We need another instance of it, so we know what the actual event type it is.
//         virtual EventType GetEventType() const override { return GetStaticType(); }
//         virtual const char* GetName() const override { return "EventType::MouseScrolled"; }

//         int GetCategoryFlags() const override{
//             return EventCategoryMouse | EventCategoryInput;
//         }


//     private:
//         float _xOffset;
//         float _yOffset;
//     };

//     // This will represent an actual base class that should not be created
//     // Hence why the constructor is protected
//     // Since this is just a base class for MouseButtonPressed and MouseButtonReleased
//     class ENGINE_API MouseButtonEvent : public Event{
//     public:
//         inline MouseCode GetMouseButton() const { return _button; }

//         // We want to set by default that whenever we are pressing/releasing the category is that this is
//         // mouse and grabbing input
//         int GetCategoryFlags() const override{
//             return EventCategoryMouse | EventCategoryInput;
//         }

//     protected:
//         // protected because we shouldn't be allowed to create this MouseButtonEvent.
//         // Only MouseButtonPressed and MouseButtonReleased can use this.
//         MouseButtonEvent(MouseCode button) : _button(button) {}

//         MouseCode _button; // containing our button hhere
//     };

//     class ENGINE_API MouseButtonPressedEvent : public MouseButtonEvent {
//     public:
//         MouseButtonPressedEvent(MouseCode button) : MouseButtonEvent(button) {}

//         std::string toString() const override {
//             // return formatCpp::format("MouseButtonPressedEvent: {}", _button);
//             std::stringstream ss;
//             ss << fmt::format("MouseButtonPressedEvent: {}", static_cast<int32_t>(_button));
//             return ss.str();
//         }

//         // In runtime we want to see what event type this is.
//         static EventType GetStaticType() { return EventType::MouseButtonPressed; }

//         // We need another instance of it, so we know what the actual event type it is.
//         virtual EventType GetEventType() const override { return GetStaticType(); }
//         virtual const char* GetName() const override { return "EventType::MouseButtonPressed"; }
//     };

//     class ENGINE_API MouseButtonReleasedEvent : public MouseButtonEvent {
//     public:
//         MouseButtonReleasedEvent(MouseCode button) : MouseButtonEvent(button) {}

//         std::string toString() const override {
//             std::stringstream ss;
//             ss << fmt::format("MouseButtonReleasedEvent: {}", static_cast<int32_t>(_button));
//             return ss.str();
//         }

//         // In runtime we want to see what event type this is.
//         static EventType GetStaticType() { return EventType::MouseButtonReleased; }

//         // We need another instance of it, so we know what the actual event type it is.
//         virtual EventType GetEventType() const override { return GetStaticType(); }
//         virtual const char* GetName() const override { return "EventType::MouseButtonReleased"; }
//     };
// };

#pragma once
#include <engine3d/Event/Event.h>
#include <engine3d/Event/MouseCodes.h>
#include <sstream>

namespace engine3d{
    class ENGINE_API MouseMovedEvent : public Event{
    public:
        MouseMovedEvent(float x, float y) : mouseX(x), mouseY(y) {};

        static EventType GetStaticType() { return EventType::MouseMoved; }

        inline float GetX() const { return mouseX; }

        inline float GetY() const { return mouseY; }

    private:
        std::string str() const override{
            std::stringstream ss;
            ss  << "MouseMovedEvent(Mouse) = (" << mouseX << ", " << mouseY << ")";
            return ss.str();
        }

        virtual EventType GetEventType() const override { return GetStaticType(); }

        int GetCategoryFlags() const override{
            return EventCategoryMouse | EventCategoryInput;
        }

    private:
        float mouseX, mouseY;
    };

    class ENGINE_API MouseScrolledEvent: public Event{
    public:
        MouseScrolledEvent(float x, float y) : xOffset(x), yOffset(y) {}

        inline float GetXOffset() const { return xOffset; }

        inline float GetYOffset() const { return yOffset; }

        static EventType GetStaticType() { return EventType::MouseScrolled; }

    private:
        std::string str() const override{
            std::stringstream ss;
            ss << "MouseScrolledEvent(mouse) = (" << xOffset << ", " << yOffset <<")";
            return ss.str();
        }

        virtual EventType GetEventType() const override { return GetStaticType(); }

        int GetCategoryFlags() const override{
            return EventCategoryMouse | EventCategoryInput;
        }

    private:
        float xOffset, yOffset;
    };

    class MouseButtonEvent : public Event{
    public:
        inline MouseCode GetMouseCode() const { return mouseCode; }

        int GetCategoryFlags() const override{
            return EventCategoryMouse | EventCategoryInput;
        }
        
    protected:
        MouseButtonEvent(MouseCode code) : mouseCode(code) {}
        MouseCode mouseCode;
    };

    class ENGINE_API MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        MouseButtonPressedEvent(MouseCode code) : MouseButtonEvent(code) {}

        static EventType GetStaticType() { return EventType::MouseButtonPressed; }

    private:
        //! @note Returns the name as a string (or any other information)
        std::string str() const override {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent(mouse) = " << static_cast<int32_t>(mouseCode);
            ss << "EventType::MouseButtonPressed";
            return ss.str();
        }

        virtual EventType GetEventType() const override { return GetStaticType(); }

        // virtual const char* GetName() const override { return "EventType::MouseButtonPressed"; }
    };

    class ENGINE_API MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        MouseButtonReleasedEvent(MouseCode code) : MouseButtonEvent(code) {}

        static EventType GetStaticType() { return EventType::MouseButtonReleased; }

    private:
        std::string str() const override {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent(Mouse) = " << static_cast<int32_t>(mouseCode);
            ss << "Event::MouseButtonReleased";
            return ss.str();
        }

        virtual EventType GetEventType() const override { return GetStaticType(); }
    };


};