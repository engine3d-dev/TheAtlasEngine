#pragma once
#include <Core/Event/Event.h>
#include <Core/Event/MouseCodes.h>
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