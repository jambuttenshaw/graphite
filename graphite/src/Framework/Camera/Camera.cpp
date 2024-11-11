#include "pch.h"
#include "Camera.h"

#include "imgui.h"

/*
 * GENERAL PERSPECTIVE EXPERIMENTS
static XMVECTOR pa{ -1.77f, 1.0f, 0.0f };
static XMVECTOR pb{ 1.77f, 1.0f, 0.0f };
static XMVECTOR pc{ -1.77f,  -1.0f, 0.0f };
static XMVECTOR pe{ 0.0f,  0.0f, -5.0f };
*/


XMFLOAT3 Camera::GetPosition() const
{
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, m_Position);
	return pos;
}

void Camera::RebuildViewIfDirty()
{
	if (m_ViewDirty)
	{
		// Recalculate properties
		const float sinY = sinf(m_Yaw);
		const float cosY = cosf(m_Yaw);
		const float sinP = sinf(m_Pitch);
		const float cosP = cosf(m_Pitch);

		m_Forward = XMVector3Normalize({ sinY * cosP, sinP, cosY * cosP });
		m_Up = { 0.0f, 1.0f, 0.0f };

		m_Right = XMVector3Normalize(XMVector3Cross(m_Up, m_Forward));
		m_Up = XMVector3Cross(m_Forward, m_Right);

		const XMVECTOR target = m_Position + m_Forward;

		m_ViewMatrix = XMMatrixLookAtLH(m_Position, target, m_Up);

		m_ViewDirty = false;
	}
}

void Camera::RebuildProjIfDirty()
{
	if (m_ProjectionDirty)
	{
		if (m_Orthographic)
		{
			m_ProjectionMatrix = XMMatrixOrthographicLH(m_OrthographicWidth, m_OrthographicHeight, m_NearPlane, m_FarPlane);
		}
		else
		{
			/*
			 * GENERAL PERSPECTIVE EXPERIMENTS
			XMVECTOR vr = XMVector3Normalize(pb - pa);
			XMVECTOR vu = XMVector3Normalize(pc - pa);
			XMVECTOR vn = XMVector3Normalize(XMVector3Cross(vr, vu));

			XMVECTOR va = pa - pe;
			XMVECTOR vb = pb - pe;
			XMVECTOR vc = pc - pe;


			float d, l, r, b, t;

			XMStoreFloat(&d, -XMVector3Dot(va, vn));

			XMStoreFloat(&l, XMVector3Dot(vr, va) * m_NearPlane / d);
			XMStoreFloat(&r, XMVector3Dot(vr, vb) * m_NearPlane / d);
			XMStoreFloat(&b, XMVector3Dot(vu, va) * m_NearPlane / d);
			XMStoreFloat(&t, XMVector3Dot(vu, vc) * m_NearPlane / d);

			const XMMATRIX P = XMMatrixPerspectiveOffCenterLH(l, r, b, t, m_NearPlane, m_FarPlane);

			XMVECTOR x{ 1.0f, 1.0f, 1.0f, 0.0f };
			XMMATRIX M(
				vr * x,
				vu * x,
				vn * x,
				XMVECTOR{ 0.0f, 0.0f, 0.0f, 1.0f }
			);
			M = XMMatrixTranspose(M);
			XMMATRIX T = XMMatrixTranslationFromVector(-pe);

			m_ProjectionMatrix = XMMatrixMultiply(XMMatrixMultiply(T, M), P);
			*/
			m_ProjectionMatrix = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
		}
	}
}

void Camera::ClampYaw()
{
	if (m_Yaw > 3.14f)
		m_Yaw = fmodf(m_Yaw, 3.14f) - 3.14f;
	if (m_Yaw < -3.13f)
		m_Yaw = 3.13f - fmodf(m_Yaw, 3.13f);
}

void Camera::ClampPitch()
{
	if (m_Pitch < -1.57f)
	{
		m_Pitch = -1.57f;
	}
	if (m_Pitch > 1.57f)
		m_Pitch = 1.57f;
}

void Camera::Gui()
{
	/*
	 * GENERAL PERSPECTIVE EXPERIMENTS
	ImGui::Begin("POV");

	auto v3_slider = [](const char* label, XMVECTOR& v, float min = -10.0f, float max = 10.0f)-> bool
		{
			XMFLOAT3 temp;
			XMStoreFloat3(&temp, v);
			if (ImGui::SliderFloat3(label, &temp.x, min, max))
			{
				v = XMLoadFloat3(&temp);
				return true;
			}
			return false;
		};

	v3_slider("pa", pa);
	v3_slider("pb", pb);
	v3_slider("pc", pc);
	v3_slider("pe", pe);

	ImGui::End();
	*/
}