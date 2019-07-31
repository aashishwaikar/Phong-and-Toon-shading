#include <Animation.hpp>

Animation::Animation(Engine::Transform* transform, float duration, int animation_type)
{
    _transform = transform;
    _duration = duration;
    _type = animation_type;
    _current = 0.0f;

    _keyframes = std::list<Keyframe>();
    _prev = _keyframes.begin();
    _next = _keyframes.begin();
}

Animation::~Animation()
{
    _keyframes.clear();
}

void Animation::AddKeyframe(float time, Engine::Transform targetTransform)
{
    bool inserted = false;
    Keyframe keyframe;
    keyframe.time = time;
    keyframe.targetTransform = targetTransform;
    for(auto it = _keyframes.begin(); it != _keyframes.end(); it++)
    {
        if (keyframe.time < it->time)
        {
            _keyframes.insert(it, keyframe);
            inserted = true;
        }
    }
    if (!inserted)
        _keyframes.push_back(keyframe);
    
    _prev = --(_keyframes.end());
    _next = _keyframes.begin();
}

void Animation::Animate(float deltaTime)
{
    if (_keyframes.size() < 2) return;

    _current += deltaTime;
	if (_current >= _duration)
	{
		_current -= _duration;
	}

    float seg_duration = _next->time - _prev->time;
    if (seg_duration < 0) seg_duration += _duration;

    float current_from_prev = _current - _prev->time;
    if (current_from_prev < 0) current_from_prev += _duration;

    float t = current_from_prev / seg_duration;
	if (t > 1.0f) {
		_prev = _next;
		_next = GetNextKeyframe(_next);
		t -= 1.0f;
	}
	Engine::Transform transform_t = Engine::Transform::Interpolate(_prev->targetTransform, _next->targetTransform, t);
	
	if (_type & AnimationType::Position) {
		_transform->SetPosition(transform_t.GetPosition());
	}
	if (_type & AnimationType::Orientation) {
		_transform->SetOrientation(transform_t.GetOrientation());
	}
	if (_type & AnimationType::Scale) {
		_transform->SetScale(transform_t.GetScale());
	}
}

std::list<Keyframe>::iterator Animation::GetNextKeyframe(std::list<Keyframe>::iterator k)
{
    auto next = ++k;
    if (next == _keyframes.end())
        next = _keyframes.begin();
    return next;
}
