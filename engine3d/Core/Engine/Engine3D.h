#pragma once

namespace engine3d{
    /**
     * @name Engine3D
     * @note Engine used for configuring entirely Engine3D.
    */
    class Engine3D{
    public:
        //! @note Initiating Engine3D for use.
        static void InitializeEngine();

        //! @note Starting Engine3D's mainloop.

        static void StartEngineMainloop();
    };
};



/*


[engine3d::InitializeCore()] -> [Engine3D::Initialize()] -> [Engine3D::StartEngineMainloop()]




*/