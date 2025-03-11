#include "graphite_pch.h"
#include "ModelLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Graphite/Core/Log.h"
#include "Graphite/RHI/Resources/Buffer.h"
#include "Graphite/RHI/Resources/VertexBuffer.h"

#include "Mesh.h"


namespace Graphite
{

	std::unique_ptr<Mesh> ModelLoader::LoadModel(const std::string& filepath)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(filepath.c_str(),
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType | 
			aiProcess_MakeLeftHanded | 
			aiProcess_FlipUVs | 
			aiProcess_FlipWindingOrder);

		if (scene == nullptr)
		{
			GRAPHITE_LOG_ERROR("Import of '{}' failed: {}", filepath.c_str(), importer.GetErrorString());
			return nullptr;
		}

		// Parse scene
		// Debug logging
		LogAssimpSceneInfo(scene, filepath);

		if (!scene->HasMeshes())
			return nullptr;

		aiMesh* mesh = scene->mMeshes[0];

		// Create model from file
		auto newMesh = std::make_unique<Mesh>();

		return std::move(newMesh);
	}


	void ModelLoader::LogAssimpSceneInfo(const aiScene* scene, const std::string& filepath)
	{
		GRAPHITE_LOG_INFO("---- Scene information: {} ----", filepath.c_str());
		GRAPHITE_LOG_INFO("Scene name: {}",			scene->mName.C_Str());
		GRAPHITE_LOG_INFO("Num meshes: {}",			scene->mNumMeshes);
		GRAPHITE_LOG_INFO("Num materials: {}",		scene->mNumMaterials);
		GRAPHITE_LOG_INFO("Num animations: {}",		scene->mNumAnimations);
		GRAPHITE_LOG_INFO("Num textures: {}",		scene->mNumTextures);
		GRAPHITE_LOG_INFO("Num lights: {}",			scene->mNumLights);
		GRAPHITE_LOG_INFO("Num cameras: {}",		scene->mNumCameras);
		GRAPHITE_LOG_INFO("Num skeletons: {}",		scene->mNumSkeletons);
		GRAPHITE_LOG_INFO("---- End scene information ----");
	}


}
