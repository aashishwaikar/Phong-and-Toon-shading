#pragma once

#include <Light.hpp>
#include <Material.hpp>

class ToonMaterial : public Engine::Material
{
public:
	void CreateMaterial(float terrain);
	void UpdateAmbientReflectance(glm::vec3 color);
	void UpdateDiffuseReflectance(glm::vec3 color);
	void UpdateSpecularReflectance(glm::vec3 color);
	void UpdateLight(std::vector<Light> &lights);
	void UpdateFogColor(glm::vec3 color);
	void UpdateTime(float time);
	void UpdateCameraPos(glm::vec3 cameraPosition);
};
