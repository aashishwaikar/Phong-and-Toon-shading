#include <ModelLoader.hpp>

#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Engine
{
	ModelLoader::ModelLoader(std::string path)
	{
		std::cout << "Loading " << path << "..." << std::endl;
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cerr << "ModelLoader error: " << importer.GetErrorString() << std::endl;
			return;
		}

		std::cout << "Load complete!" << std::endl;
		std::cout << " * Total meshes: " << scene->mNumMeshes << std::endl;
	}

	void ModelLoader::GenerateMesh(Mesh *_mesh, unsigned int meshIdx)
	{
		const aiScene *scene = importer.GetScene();
		if (!scene)
		{
			std::cerr << "GenerateMesh error: model is not loaded yet!" << std::endl;
			return;
		}
		if (meshIdx < 0 || meshIdx >= scene->mNumMeshes)
		{
			std::cerr << "GenerateMesh error: meshIdx out of bounds!" << std::endl;
			return;
		}

		aiMesh *mesh = scene->mMeshes[meshIdx];
		bool hasUV = mesh->mTextureCoords[0];
		
		_mesh->AddAttribute(4);	// position
		_mesh->AddAttribute(4); // normal
		for (size_t i = 0; i < mesh->mNumVertices; i++)
		{
			_mesh->AddVertexData(mesh->mVertices[i].x);
			_mesh->AddVertexData(mesh->mVertices[i].y);
			_mesh->AddVertexData(mesh->mVertices[i].z);
			_mesh->AddVertexData(1.0f);
			_mesh->AddVertexData(mesh->mNormals[i].x);
			_mesh->AddVertexData(mesh->mNormals[i].y);
			_mesh->AddVertexData(mesh->mNormals[i].z);
			_mesh->AddVertexData(0.0f);
		}
		_mesh->SetHasIndex(true);
		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (size_t j = 0; j < face.mNumIndices; j++)
			{
				_mesh->AddIndexData(face.mIndices[j]);
			}
		}
		_mesh->SetNumElements(mesh->mNumFaces * 3);
		_mesh->CreateMesh();

		std::cout << "Mesh generation complete!" << std::endl;
		std::cout << " * Vertices: " << mesh->mNumVertices << std::endl;
		std::cout << " * Triangles: " << mesh->mNumFaces << std::endl;
	}
}
