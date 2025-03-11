#pragma once

#include "Graphite/Core/Core.h"

// Assimp scene structure
struct aiScene;


namespace Graphite
{
	class Mesh;

	class ModelLoader
	{
	public:
		static GRAPHITE_API std::unique_ptr<Mesh> LoadModel(const std::string& filepath);


	private:
		static void LogAssimpSceneInfo(const aiScene* scene, const std::string& filepath);
	};

}
