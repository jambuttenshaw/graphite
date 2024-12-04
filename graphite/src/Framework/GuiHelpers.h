#pragma once

#include "Core.h"
#include "imgui.h"
#include "imgui_internal.h"

#include "Math.h"


namespace GuiHelpers
{
	// Adds a scoped ID to ImGui's ID stack
	template<typename T>
	class IDScope
	{
	public:
		IDScope(T id)
			: m_ID(id)
		{
			if (m_ID)
			{
				ImGui::PushID(m_ID);
			}
		}
		~IDScope()
		{
			if (m_ID)
			{
				ImGui::PopID();
			}
		}

		DISALLOW_COPY(IDScope)
		DISALLOW_MOVE(IDScope)

	private:
		T m_ID = nullptr;
	};

	// Disables all ImGui items in the scope, can be conditional
	class DisableScope
	{
	public:
		DisableScope(bool condition = true)
			: m_Cond(condition)
		{
			if (m_Cond)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}
		}
		~DisableScope()
		{
			if (m_Cond)
			{
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}
		}

		DISALLOW_COPY(DisableScope)
		DISALLOW_MOVE(DisableScope)

	private:
		bool m_Cond = false;

	};

	// Returns true when the flag was modified
	inline bool FlagOption(UINT* bitfield, const char* name, UINT value)
	{
		ASSERT(bitfield, "Invalid bitfield");
		bool flag = *bitfield & value;
		if (ImGui::Checkbox(name, &flag))
		{
			if (flag)
				*bitfield |= value;
			else
				*bitfield &= ~value;
			return true;
		}
		return false;
	}

	inline bool DirectionAsSphericalCoordinates(const char* id, XMFLOAT3& inOutDirection)
	{
		IDScope idScope(id);

		bool newDir = false;
		float phi, theta;

		Math::CartesianDirectionToSpherical(inOutDirection, phi, theta);

		newDir |= ImGui::SliderAngle("Theta", &theta, 1.0f, 179.0f);
		newDir |= ImGui::SliderAngle("Phi", &phi, -179.0f, 180.0f);
		if (newDir)
		{
			const float sinTheta = sinf(theta);
			const float cosTheta = cosf(theta);
			const float sinPhi = sinf(phi);
			const float cosPhi = cosf(phi);

			inOutDirection.x = sinTheta * cosPhi;
			inOutDirection.y = cosTheta;
			inOutDirection.z = sinTheta * sinPhi;
		}

		return newDir;
	}

}
