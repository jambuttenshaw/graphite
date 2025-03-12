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
		{
			GRAPHITE_LOG_WARN("Scene did not contain any meshes.");
			return nullptr;
		}

		aiMesh* mesh = scene->mMeshes[0];
		GRAPHITE_LOG_INFO("Num faces: {}", mesh->mNumFaces);

		if (mesh->mNumFaces == 0)
		{
			GRAPHITE_LOG_WARN("Mesh did not have any faces.");
			return nullptr;
		}


		// Create input layout
		std::vector<InputElementDesc> inputElements;
		if (mesh->HasPositions())
		{
			inputElements.emplace_back(VertexAttribute::Position, GraphiteFormat_R32G32B32_FLOAT);
		}
		if (mesh->HasNormals())
		{
			inputElements.emplace_back(VertexAttribute::Normal, GraphiteFormat_R32G32B32_FLOAT);
		}

		// Create input layout
		InputLayout inputLayout(inputElements);

		// Create model from file
		auto newMesh = std::make_unique<Mesh>(mesh->mNumVertices, 3 * mesh->mNumFaces, std::move(inputLayout));
		newMesh->GetVertexBuffer()->CopyAttribute(VertexAttribute::Position, std::span<const aiVector3D>{ mesh->mVertices, mesh->mNumVertices });
		newMesh->GetVertexBuffer()->CopyAttribute(VertexAttribute::Normal, std::span<const aiVector3D>{ mesh->mNormals, mesh->mNumVertices });

		uint32_t startElement = 0;
		for (uint32_t faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++)
		{
			aiFace face = mesh->mFaces[faceIdx];

			GRAPHITE_ASSERT(face.mNumIndices == 3, "Only triangles are supported!");
			newMesh->GetIndexBuffer()->CopyElements(startElement, face.mNumIndices, 0, face.mIndices, face.mNumIndices * sizeof(unsigned int));
			startElement += face.mNumIndices;
		}

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
