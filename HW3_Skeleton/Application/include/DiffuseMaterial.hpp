#pragma once

#include <Light.hpp>
#include <Material.hpp>

class DiffuseMaterial : public Engine::Material
{
private:
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
public:
    void CreateMaterial();
    void UpdateDiffuseReflectance(glm::vec3 color);
	void UpdateLight(std::vector<Light> &lights);
};
