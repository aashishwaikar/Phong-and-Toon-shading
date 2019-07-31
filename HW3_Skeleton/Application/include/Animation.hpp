#pragma once

#include <vector>
#include <list>

#include <RenderObject.hpp>

/*
	Wow, you might be really interested in this class!
	Here's some detailed explanation on Animation.
*/

enum AnimationType: int
{
/*
	AnimationType describes what is affected by Animation.
	Animation can affect position, orientation, scale, and any combination of three.
	Combinations are done by 'bitwise or' operation
	Example combinations are shown below:

	position only - AnimationType::Position
	position and scale - AnimationType::Position | AnimationType::Scale
	position and orientation and scale - AnimationType::All

	It will be useful if you want to combine multiple movements.
	For example, consider a bunny moving from A to B in 10 seconds, and while moving,
	the bunny spins along y axis in every 1 seconds.
	You can animate this using 2 animations affecting each position and orientation:

	// in InitScene()
	...
	moving_animation = Animation(bunny->GetTransform(), 10, AnimationType::Position);
	...
	spinning_animation = Animation(bunny->GetTransform(), 1, AnimationType::Orientation);
	...
	
	// in a main loop
	...
	moving_animation.Animate(elapsed_time);
	spinning_animation.Animate(elapsed_time);
	...

	Unfortunately, you cannot run 2 or more animation of same transform affecting same field.
	Only the animation that you invoked Animate() at last will be applied.
*/
	None = 0,
    Position = 1,
    Orientation = 2,
    Scale = 4,
	All = 7,
};

struct Keyframe
{
/*
	This struct is for an internal use.
*/
    float time;
    Engine::Transform targetTransform;
};

class Animation
{
private:
    float _duration;
    float _current;
    Engine::Transform* _transform;
    int _type;
    std::list<Keyframe> _keyframes;
    std::list<Keyframe>::iterator _prev;
    std::list<Keyframe>::iterator _next;

    std::list<Keyframe>::iterator GetNextKeyframe(std::list<Keyframe>::iterator k);
public:
/*
	These 4 method are all you need to know (if you decided to use Animation - it's optional)
	It is fairly easy and intuitive to use, so why don't you just try different things with it?

	If you want to improve Animation by yourself, good news for you.
	There are tons of improvements you can do right now!
	But please remind that the implementation of Animation is out of scope of the homework,
		and any work on Animation.hpp or Animation.cpp will not affect grades.
	(Meaning that if you want to seek for creativity score, find elsewhere!)
*/
    Animation(Engine::Transform* transform, float duration, int animation_type);
    ~Animation();
    void AddKeyframe(float time, Engine::Transform targetTransform);
    void Animate(float deltaTime);

	float GetTime() { return _current; }
};
