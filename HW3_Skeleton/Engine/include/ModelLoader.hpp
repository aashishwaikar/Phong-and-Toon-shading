#pragma once

#include <string>

#include <Mesh.hpp>
#include <assimp/Importer.hpp>

namespace Engine
{
	class ModelLoader
	{
	private:
		Assimp::Importer importer;

	public:
		ModelLoader(std::string path);
		void GenerateMesh(Mesh *mesh, unsigned int meshIdx);
	};
}
