#include <Camera.hpp>
#include <iostream>

namespace Engine
{
    Camera::Camera()
    {
    }

    Camera::Camera(glm::vec3 position, glm::mat4 orientation)
    {
        _transform.SetPosition(position);
        _transform.SetOrientation(orientation);
    }

    void Camera::SetProjection(float aspectRatio, float fov, float zNear, float zFar) 
    { 
        _projection.aspectRatio = aspectRatio;
        _projection.fov = fov;
        _projection.zNear = zNear;
        _projection.zFar = zFar;
    }

    ProjectionData Camera::GetProjection()
    {
        return _projection;
    }
} // Engine