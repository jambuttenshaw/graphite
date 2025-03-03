#pragma once


namespace Graphite
{

	/**
	 * A scene is a container for all objects in the scene.
	 * 
	 */
	class Scene
	{
	public:

		std::span<const Entity> GetEntities() const { return m_Entities; }

	private:
		// Collection of all entities in the scene
		std::vector<Entity> m_Entities;
	};

}
