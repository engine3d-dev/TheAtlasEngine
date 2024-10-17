#include <glm/fwd.hpp>
namespace engine3d 
{
    class SyncUpdate
    {
        public:
            void runTickUpdate(float deltaTime);
            void runUpdate(float deltaTime);
            void runLateUpdate(float deltaTime);

        protected:
            virtual void onTickUpdate(float deltaTime) {};
            virtual void update(float deltaTime) {};
            virtual void lateUpdate(float deltaTime) {}; 
    };
};