#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Engine
{
    class Transform
    {
    private:
        glm::vec3 _position = glm::vec3(0.0f);
        glm::mat4 _orientation = glm::mat4(1.0f);
        glm::vec3 _scale = glm::vec3(1.0f);

        Transform *parent = nullptr;
    public:
        static glm::vec3 InterpolatePosition(Transform &zero, Transform &one, float t);
        static glm::mat4 InterpolateOrientation(Transform &zero, Transform &one, float t);
        static glm::vec3 InterpolateScale(Transform &zero, Transform &one, float t);
        static Transform Interpolate(Transform &zero, Transform &one, float t);

		/*
		^	What you do not need to know
		|
		+-----------------------------
		|
		v	What you probably need to know

		*/

        glm::vec3 GetPosition() { return _position; }
        void SetPosition(glm::vec3 position) { _position = position; }
        glm::mat4 GetOrientation() { return _orientation; }
        void SetOrientation(glm::mat4 orientation) { _orientation = orientation; }
        glm::vec3 GetScale() { return _scale; }
        void SetScale(glm::vec3 scale) { _scale = scale; }
        glm::mat4 GetWorldTransform();

		void AddParent(Transform *parent_transform) { parent = parent_transform; };
        void LookAt(glm::vec3 dir, glm::vec3 up);
    };
}