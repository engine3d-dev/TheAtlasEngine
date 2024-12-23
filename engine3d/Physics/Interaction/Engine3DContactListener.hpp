#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>


#include <print>

using namespace JPH;

namespace engine3d
{
    class Engine3DContactlistener : public ContactListener
    {
        public:

            std::vector<std::function<void()>> m_ContactSubscibers;
        //Component Subscription
        template<typename UComponent, typename UFunction, typename... Args>
        void Subscribe(UComponent* p_Instance, const UFunction&& p_Update)
        {

            if constexpr (m_HasContact<UComponent>::value)
            {
                if(&UComponent::Update == p_Update)
                {
                    m_ContactSubscibers.push_back([p_Instance, p_Update]() 
                    {
                        (p_Instance->*p_Update)();
                    });
                    return;
                }
            }
        }

        private:

        //! @note update specialization
        template<typename, typename = std::void_t<>>
        struct m_HasContact : std::false_type{};

        template<typename UCompClass>
        struct m_HasContact<UCompClass, std::void_t<decltype(
            std::declval<UCompClass>().ContactPersisted())>> : std::true_type {};
        virtual ValidateResult	OnContactValidate(const Body &inBody1, const Body &inBody2, RVec3Arg inBaseOffset, const CollideShapeResult &inCollisionResult) override
        {
            std::print("Validating...\n");

            // Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
            return ValidateResult::AcceptAllContactsForThisBodyPair;
        }

        virtual void OnContactAdded(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override
        {
            
        }

        virtual void OnContactPersisted(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override
        {
            
        }

        virtual void OnContactRemoved(const SubShapeIDPair &inSubShapePair) override
        {
            std::print("Contact was removed!\n");
        }
    };
};