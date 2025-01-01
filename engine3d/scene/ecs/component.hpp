#pragma once

namespace engine3d{
    /**
     * @name Component
     * @note This wrapper is to help register how both metadata components and active components may work with registry
    */
    template<typename T>
    class Component{
    public:
        T& Get(){}

        void Add(const T&){}

        bool Has(){ return false; }
    };
};