#pragma once
#include <Core/core.h>
#include <Core/Event/Event.h>
#include <sstream>

namespace engine3d{

    class WindowResizeEvent : public Event{
    public:
        WindowResizeEvent(uint32_t w, uint32_t h) : width(w), height(h) {}

        uint32_t GetWidth() const { return width; }

        uint32_t GetHeight() const{ return height; }

        static EventType GetStaticType() { return EventType::WindowResize; }

    private:

        EventType GetEventType() const override {
            return GetStaticType();
        }

        std::string str() const override{
            std::stringstream ss;
            ss << "WindowResizeEvent (" << width << ", " << height << ")";
            ss << "EventType::WindowResize";
            return ss.str();
        }

        int GetCategoryFlags() const override{
            return EventCategoryApplication;
        }

    private:
        uint32_t width, height;
    };

    class ENGINE_API WindowCloseEvent : public Event{
    public:
        WindowCloseEvent(){}

        static EventType GetStaticType() { return EventType::WindowClose; }

    private:

        std::string str() const override{
            std::stringstream ss;
            ss << "EventType::WindowClose";
            return ss.str();
        }

        EventType GetEventType() const override { return GetStaticType(); }

        int GetCategoryFlags() const override{
            return EventCategoryApplication;
        }
    };

    // class ENGINE_API ApplicationTickEvent : public Event{
    // public:
    //     ApplicationTickEvent(){}

    //     static EventType GetStaticType() { return EventType::AppTick; }

    // private:
    //     std::string str() const override{
    //         std::stringstream ss;
    //         ss << "EventType::AppTick";
    //         return ss.str();
    //     }

    //     EventType GetEventType() const override { return GetStaticType(); }

    //     int GetCategoryFlags() const override{
    //         return EventCategoryApplication;
    //     }
    // };

};