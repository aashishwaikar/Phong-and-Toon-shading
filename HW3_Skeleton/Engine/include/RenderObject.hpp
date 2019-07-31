#pragma once

#include <glm/glm.hpp>

#include <Camera.hpp>
#include <Mesh.hpp>
#include <Material.hpp>
#include <Transform.hpp>

namespace Engine
{
    class RenderObject
	{
    protected:
        RenderObject();
        Mesh* _mesh;
        Material* _material;
		Transform _transform;
		
		RenderObject *parent = NULL;

    public:
        RenderObject(Mesh* mesh, Material* material);
        void Render(Camera* cam);
        void SetMesh(Mesh* mesh) { _mesh = mesh; }
        void SetMaterial(Material* material) { _material = material; }
        
        Transform* GetTransform() { return &_transform; }
        void SetTransform(Transform t) { _transform = t; }
    };
}