#include <Transform.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Engine
{
    glm::mat4 Transform::GetWorldTransform()
    {
        // Compute TRS
		glm::mat4 parent_scale = glm::mat4(1.0f);
		glm::mat4 parent_WorldTransform = glm::mat4(1.0f);
		if (parent != nullptr)
		{
			parent_scale = glm::scale(glm::mat4(1.0f), parent->GetScale());
			parent_WorldTransform = parent->GetWorldTransform();
		}

        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), _position);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), _scale);
        return parent_WorldTransform * glm::inverse(parent_scale) * translationMatrix * _orientation * scaleMatrix;
    }

    void Transform::LookAt(glm::vec3 dir, glm::vec3 up)
    {
        /*
            LookAt function changes orientation of Transform
            dir - target +z axis
            up  - target +y axis
        */
        dir = glm::normalize(dir);
        up = glm::normalize(up);

        if (glm::length(glm::cross(up, dir)) == 0)
        {
            // Invalid parameters
            return;
        }

        glm::vec3 z_axis = dir;
        glm::vec3 x_axis = glm::normalize(glm::cross(up, z_axis));
        glm::vec3 y_axis = glm::normalize(glm::cross(z_axis, x_axis));
        glm::mat4 rot_3 = glm::mat3(x_axis, y_axis, z_axis);
        _orientation = glm::mat4(rot_3);
    }

    Transform Transform::Interpolate(Transform &zero, Transform &one, float t)
    {
        Transform result;
        result._position = InterpolatePosition(zero, one, t);
        result._orientation = InterpolateOrientation(zero, one, t);
        result._scale = InterpolateScale(zero, one, t);
		
		return result;
    }

    glm::vec3 Transform::InterpolatePosition(Transform &zero, Transform &one, float t)
    {
        return glm::mix(zero._position, one._position, t);
    }
    
    glm::mat4 Transform::InterpolateOrientation(Transform &zero, Transform &one, float t)
    {
        glm::quat zero_quat = glm::quat_cast(zero._orientation);
        glm::quat one_quat = glm::quat_cast(one._orientation);
        glm::quat t_quat = glm::slerp(zero_quat, one_quat, t);
        return glm::mat4_cast(t_quat);
    }
    
    glm::vec3 Transform::InterpolateScale(Transform &zero, Transform &one, float t)
    {
        return glm::mix(zero._scale, one._scale, t);
    }
}