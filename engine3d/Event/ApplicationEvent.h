// #pragma once
// #include <engine3d/Event/Event.h>
// #include <sstream>

// namespace engine3d{
//     // Basically we assign to which event which
//     //  Handling events for resizing the window.
//     class ENGINE_API WindowResizeEvent : public Event{
//     public:
//         WindowResizeEvent(unsigned int w, unsigned int h) : _width(w), _height(h) {}

//         inline unsigned int GetWidth() const { return _width; }
//         inline unsigned int GetHeight() const { return _height; }

//         std::string toString() const  override {
//             // return formatCpp::format("WindowResizeEvent: (w={}, h={})", _width, _height);
//             std::stringstream ss;
//             ss << "WindowResizeEvent: " << _width << ", " << _height;
//             return ss.str();
//         }

//         // In runtime we want to see what event type this is.
//         static EventType GetStaticType() { return EventType::WindowResize; }

//         // We need another instance of it, so we know what the actual event type it is.
//         virtual EventType GetEventType() const override { return GetStaticType(); }
//         virtual const char* GetName() const override { return "EventType::WindowResize"; }

//         int GetCategoryFlags() const override{
//             return EventCategoryApplication;
//         }

//         friend std::ostream& operator<<(std::ostream& outs, const WindowResizeEvent& e){
//             return outs << e.toString();
//         }

//     private:
//         unsigned int _width;
//         unsigned int _height;
//     };

//     // Handling the Window closing event
//     // Does not need much data at all.
//     class ENGINE_API WindowCloseEvent : public Event{
//     public:
//         WindowCloseEvent() {}

//         // In runtime we want to see what event type this is.
//         static EventType GetStaticType() { return EventType::WindowClose; }

//         // We need another instance of it, so we know what the actual event type it is.
//         virtual EventType GetEventType() const override { return GetStaticType(); }
//         virtual const char* GetName() const override { return "EventType::WindowClose"; }

//         int GetCategoryFlags() const override{
//             return EventCategoryApplication;
//         }
//     };


//     // Handling the application local events here.
//     // Idea is that thhese are here to implement, in case they are actually needed
//     // to be implemented.
//     class ENGINE_API AppTickEvent : public Event{
//     public:
//         AppTickEvent() {}

//         // In runtime we want to see what event type this is.
//         static EventType GetStaticType() { return EventType::AppTick; }

//         // We need another instance of it, so we know what the actual event type it is.
//         virtual EventType GetEventType() const override { return GetStaticType(); }
//         virtual const char* GetName() const override { return "EventType::AppTick"; }

//         int GetCategoryFlags() const override{
//             return EventCategoryApplication;
//         }
//     };

//     class ENGINE_API AppUpdateEvent : public Event{
//     public:
//         AppUpdateEvent() {}

//         // In runtime we want to see what event type this is.
//         static EventType GetStaticType() { return EventType::AppUpdate; }

//         // We need another instance of it, so we know what the actual event type it is.
//         virtual EventType GetEventType() const override { return GetStaticType(); }
//         virtual const char* GetName() const override { return "EventType::AppUpdate"; }

//         int GetCategoryFlags() const override{
//             return EventCategoryApplication;
//         }
//     };

//     class ENGINE_API AppRenderEvent : public Event{
//     public:
//         // In runtime we want to see what event type this is.
//         static EventType GetStaticType() { return EventType::AppRender; }

//         // We need another instance of it, so we know what the actual event type it is.
//         virtual EventType GetEventType() const override { return GetStaticType(); }
//         virtual const char* GetName() const override { return "EventType::AppRender"; }

//         int GetCategoryFlags() const override{
//             return EventCategoryApplication;
//         }
//     };

// };

#pragma once
#include <engine3d/Core/core.h>
#include <engine3d/Event/Event.h>
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

        // const char* GetName() const override { return "EventType::WindowResizeEvent"; }

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

        // const char* GetName() const override { return "EventType::WindowCloseEvent"; }

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

    // class ENGINE_API ApplicationRend
};