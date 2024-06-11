#pragma once
#include <engine3d/Core/core.h>
#include <engine3d/Event/Event.h>
#include <engine3d/Event/KeyCodes.h>
#include <sstream>

namespace engine3d{
    
    class ENGINE_API KeyEvent : public Event{
    public:

        inline KeyCode GetKeyCode() const { return keycode; }

        int GetCategoryFlags() const override{
            return EventCategoryKeyboard | EventCategoryInput;
        }

    protected:
        KeyEvent(KeyCode code) : keycode(code) {}

        KeyCode keycode;
    };

    class ENGINE_API KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(KeyCode code, int countRepeated) : KeyEvent(code), repeated(countRepeated) {}

        inline int GetRepeatedCount() const { return repeated; }

        static EventType GetStaticType() { return EventType::KeyPressed; }

    private:
        std::string str() const override{
            std::stringstream ss;
            ss << "KeyPressedEvent(Key) = " << static_cast<int32_t>(keycode);
            ss << "EventType::KeyPressedEvent";
            return ss.str();
        }

        // const char* GetName() const override { return "EventType::KeyPressedEvent"; }

        EventType GetEventType() const override { return GetStaticType(); }

    private:
        int repeated;
    };

    class ENGINE_API KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(KeyCode code) : KeyEvent(code){}

        static EventType GetStaticType() { return EventType::KeyReleased; }

    private:
        std::string str() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent(Key) = " << static_cast<int32_t>(keycode);
            return ss.str();
        }

        EventType GetEventType() const override { return GetStaticType(); }

        // const char* GetName() const override { return "EventType::KeyReleasedEvent"; }
    };

    class ENGINE_API KeyTypedEvent : public KeyEvent {
    public:
        KeyTypedEvent(KeyCode code) : KeyEvent(code) {}

        static EventType GetStaticType() { return EventType::KeyTyped; }

    private:
        std::string str() const override{
            std::stringstream ss;
            ss << "KeyTypedEvent(key) = " << static_cast<int32_t>(keycode);
            return ss.str();
        }

        EventType GetEventType() const override { return GetStaticType(); }
        // const char* GetName() const override { return "EventType::KeyTypedEvent"; }
    };


};