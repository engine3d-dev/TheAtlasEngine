# New Notes

## Keep in Mind Perspective

I actually thought more on our conversation, and realized that the way we are thinking about this is entirely wrong.

You know what?! After doing more digging I dont think that with components that is something we should have as shared data.


## Topic: When talking about this again.

To make sure that we are on the same page, and be clean. These are why you think game component should be referenced to the scene object, correct?

## Foundational Proposal of this Entity Component System.
- 

## Following Reasons Below

These reasons below are why zach want's to reference the scene object with components.

1.) O(N) access
2.) Because we can allow multiple threads to modify shared data in between components.
3.) When components share the same scene object, you can add modifications to those components via different threads.
4.) Reusability and Flexability for users to adding/removing tasks.
5.) Using the "method" of a task graph to "hopefully" make sure that we are running into issues with accessing. Meaning if we have a task that runs  


### Example

This is the layout of how the components would store scene object references to the objects they are associated with.

class GameComponent{
    shared_ptr<SceneObject> ReferenceToSceneObject;
};


class TransformComponent{
    void SetReferenceToSceneObject(shared_ptr<SceneObject> p_ObjectAssignedTo){
        ReferenceToSceneObject = p_ObjectAssignedTo;
    }
};

class CameraComponent : public GameComponent{
    void SetReferenceToSceneObject(shared_ptr<SceneObject> p_ObjectAssignedTo){
        ReferenceToSceneObject = p_ObjectAssignedTo;
    }
};

// non-movable static scene object component. Meaning static scene object that are idle or dont move/no dynamic.
class StaticPhysicComponent : public GameComponent{
    void SetReferenceToSceneObject(shared_ptr<SceneObject> p_ObjectAssignedTo){
        ReferenceToSceneObject = p_ObjectAssignedTo;
    }
};

// movable object that is a scene object that can become dynamic in a given scene
class DynamicPhysicComponent : public GameComponent{
    void SetReferenceToSceneObject(shared_ptr<SceneObject> p_ObjectAssignedTo){
        ReferenceToSceneObject = p_ObjectAssignedTo;
    }
};

void SceneObject::AddComponent(CameraComponent component){
    component.SetReferenceToSceneObject(this);
}



Here are reasons why I think we should take a step back and re-evaluate what and how we are approaching this:

[Note] - will ask Khalil if he's able to hop into our discussion on this topic do help validate our concerns.


1.) Multi-threading with shared state
- Leading to increase complexity, hrad to understand bugs, and generally problems all over the place

2.) Parallel Stuff works better when data is fully separate
- Each thread has as little dependency as possible on others.

3.) Memory Operation
- Moving around the memory ends up using more time than the operation itself. You basically are having it more the whole memory around 4x. To make it quicker, they should either be operated in sections.

4.) Memory Overhead
- Commonly you want your memory use overhead to be low relative to the processing to make the most of multithreaded operations.
- It is a more concern with limited memory access rather then CPU compute power.








## Use of std::span within SyncUpdate, GLobalUpdate

### Here are my Reasons

we should treat every object as 