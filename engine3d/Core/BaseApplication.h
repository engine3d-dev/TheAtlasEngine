#pragma once

namespace engine3d{
    class BaseApplication{
    public:
        void InitiateApplication() { InitializeCore(); }
        void StartApplication() { StartApplicationCore(); }
    private:
        virtual void InitializeCore() = 0;
        virtual void StartApplicationCore() = 0;
    };
};