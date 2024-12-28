#pragma once
// #include "engine_logger.hpp"
#include <core/engine_logger.hpp>
#include <thread>

namespace engine3d{
    class Thread{
    public:
        Thread() = delete;
        Thread(const std::string& p_Tag="Undefined") : m_Tag(p_Tag) {}
        
        ~Thread(){
            ConsoleLogInfo("Are you destructed!");
            if(this->Joinable()){
                this->Join();
            }
        }

        template<typename T, typename... Args>
        void Dispatch(T&& func, Args&&... args){
            m_Thread = std::thread(func, std::forward<Args>(args)...);
        }

        bool Joinable(){
            return m_Thread.joinable();
        }

        void Join(){
            m_Thread.join();
        }

    private:
        std::string m_Tag;
        std::thread m_Thread;
    };
};