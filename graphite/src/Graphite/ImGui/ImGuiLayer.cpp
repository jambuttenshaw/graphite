#include "graphite_pch.h"
#include "ImGuiLayer.h"

#include "Graphite/Core/Application.h"
#include "Graphite/Core/Platform.h"
#include "Graphite/Core/Assert.h"
#include "Graphite/RHI/GraphicsContext.h"

#include "imgui.h"
#include "ImGuiBackend.h"
#include "Graphite/RHI/CommandRecordingContext.h"


static ImGuiKey GraphiteKeyToImGuiKey(Graphite::Key key)
{
	switch (key)
	{
	default: return ImGuiKey_None;
	case Graphite::Key_None: return ImGuiKey_None;
	case Graphite::Key_Tab: return ImGuiKey_Tab;
	case Graphite::Key_LeftArrow: return ImGuiKey_LeftArrow;
	case Graphite::Key_RightArrow: return ImGuiKey_RightArrow;
	case Graphite::Key_UpArrow: return ImGuiKey_UpArrow;
	case Graphite::Key_DownArrow: return ImGuiKey_DownArrow;
	case Graphite::Key_PageUp: return ImGuiKey_PageUp;
	case Graphite::Key_PageDown: return ImGuiKey_PageDown;
	case Graphite::Key_Home: return ImGuiKey_Home;
	case Graphite::Key_End: return ImGuiKey_End;
	case Graphite::Key_Insert: return ImGuiKey_Insert;
	case Graphite::Key_Delete: return ImGuiKey_Delete;
	case Graphite::Key_Backspace: return ImGuiKey_Backspace;
	case Graphite::Key_Space: return ImGuiKey_Space;
	case Graphite::Key_Enter: return ImGuiKey_Enter;
	case Graphite::Key_Escape: return ImGuiKey_Escape;
	case Graphite::Key_LeftCtrl: return ImGuiKey_LeftCtrl;
	case Graphite::Key_LeftShift: return ImGuiKey_LeftShift;
	case Graphite::Key_LeftAlt: return ImGuiKey_LeftAlt;
	case Graphite::Key_LeftSuper: return ImGuiKey_LeftSuper;
	case Graphite::Key_RightCtrl: return ImGuiKey_RightCtrl;
	case Graphite::Key_RightShift: return ImGuiKey_RightShift;
	case Graphite::Key_RightAlt: return ImGuiKey_RightAlt;
	case Graphite::Key_RightSuper: return ImGuiKey_RightSuper;
	case Graphite::Key_Menu: return ImGuiKey_Menu;
	case Graphite::Key_0: return ImGuiKey_0;
	case Graphite::Key_1: return ImGuiKey_1;
	case Graphite::Key_2: return ImGuiKey_2;
	case Graphite::Key_3: return ImGuiKey_3;
	case Graphite::Key_4: return ImGuiKey_4;
	case Graphite::Key_5: return ImGuiKey_5;
	case Graphite::Key_6: return ImGuiKey_6;
	case Graphite::Key_7: return ImGuiKey_7;
	case Graphite::Key_8: return ImGuiKey_8;
	case Graphite::Key_9: return ImGuiKey_9;
	case Graphite::Key_A: return ImGuiKey_A;
	case Graphite::Key_B: return ImGuiKey_B;
	case Graphite::Key_C: return ImGuiKey_C;
	case Graphite::Key_D: return ImGuiKey_D;
	case Graphite::Key_E: return ImGuiKey_E;
	case Graphite::Key_F: return ImGuiKey_F;
	case Graphite::Key_G: return ImGuiKey_G;
	case Graphite::Key_H: return ImGuiKey_H;
	case Graphite::Key_I: return ImGuiKey_I;
	case Graphite::Key_J: return ImGuiKey_J;
	case Graphite::Key_K: return ImGuiKey_K;
	case Graphite::Key_L: return ImGuiKey_L;
	case Graphite::Key_M: return ImGuiKey_M;
	case Graphite::Key_N: return ImGuiKey_N;
	case Graphite::Key_O: return ImGuiKey_O;
	case Graphite::Key_P: return ImGuiKey_P;
	case Graphite::Key_Q: return ImGuiKey_Q;
	case Graphite::Key_R: return ImGuiKey_R;
	case Graphite::Key_S: return ImGuiKey_S;
	case Graphite::Key_T: return ImGuiKey_T;
	case Graphite::Key_U: return ImGuiKey_U;
	case Graphite::Key_V: return ImGuiKey_V;
	case Graphite::Key_W: return ImGuiKey_W;
	case Graphite::Key_X: return ImGuiKey_X;
	case Graphite::Key_Y: return ImGuiKey_Y;
	case Graphite::Key_Z: return ImGuiKey_Z;
	case Graphite::Key_F1: return ImGuiKey_F1;
	case Graphite::Key_F2: return ImGuiKey_F2;
	case Graphite::Key_F3: return ImGuiKey_F3;
	case Graphite::Key_F4: return ImGuiKey_F4;
	case Graphite::Key_F5: return ImGuiKey_F5;
	case Graphite::Key_F6: return ImGuiKey_F6;
	case Graphite::Key_F7: return ImGuiKey_F7;
	case Graphite::Key_F8: return ImGuiKey_F8;
	case Graphite::Key_F9: return ImGuiKey_F9;
	case Graphite::Key_F10: return ImGuiKey_F10;
	case Graphite::Key_F11: return ImGuiKey_F11;
	case Graphite::Key_F12: return ImGuiKey_F12;
	case Graphite::Key_F13: return ImGuiKey_F13;
	case Graphite::Key_F14: return ImGuiKey_F14;
	case Graphite::Key_F15: return ImGuiKey_F15;
	case Graphite::Key_F16: return ImGuiKey_F16;
	case Graphite::Key_F17: return ImGuiKey_F17;
	case Graphite::Key_F18: return ImGuiKey_F18;
	case Graphite::Key_F19: return ImGuiKey_F19;
	case Graphite::Key_F20: return ImGuiKey_F20;
	case Graphite::Key_F21: return ImGuiKey_F21;
	case Graphite::Key_F22: return ImGuiKey_F22;
	case Graphite::Key_F23: return ImGuiKey_F23;
	case Graphite::Key_F24: return ImGuiKey_F24;
	case Graphite::Key_Apostrophe: return ImGuiKey_Apostrophe;
	case Graphite::Key_Comma: return ImGuiKey_Comma;
	case Graphite::Key_Minus: return ImGuiKey_Minus;
	case Graphite::Key_Period: return ImGuiKey_Period;
	case Graphite::Key_Slash: return ImGuiKey_Slash;
	case Graphite::Key_Semicolon: return ImGuiKey_Semicolon;
	case Graphite::Key_Equal: return ImGuiKey_Equal;
	case Graphite::Key_LeftBracket: return ImGuiKey_LeftBracket;
	case Graphite::Key_Backslash: return ImGuiKey_Backslash;
	case Graphite::Key_RightBracket: return ImGuiKey_RightBracket;
	case Graphite::Key_GraveAccent: return ImGuiKey_GraveAccent;
	case Graphite::Key_CapsLock: return ImGuiKey_CapsLock;
	case Graphite::Key_ScrollLock: return ImGuiKey_ScrollLock;
	case Graphite::Key_NumLock: return ImGuiKey_NumLock;
	case Graphite::Key_PrintScreen: return ImGuiKey_PrintScreen;
	case Graphite::Key_Pause: return ImGuiKey_Pause;
	case Graphite::Key_Keypad0: return ImGuiKey_Keypad0;
	case Graphite::Key_Keypad1: return ImGuiKey_Keypad1;
	case Graphite::Key_Keypad2: return ImGuiKey_Keypad2;
	case Graphite::Key_Keypad3: return ImGuiKey_Keypad3;
	case Graphite::Key_Keypad4: return ImGuiKey_Keypad4;
	case Graphite::Key_Keypad5: return ImGuiKey_Keypad5;
	case Graphite::Key_Keypad6: return ImGuiKey_Keypad6;
	case Graphite::Key_Keypad7: return ImGuiKey_Keypad7;
	case Graphite::Key_Keypad8: return ImGuiKey_Keypad8;
	case Graphite::Key_Keypad9: return ImGuiKey_Keypad9;
	case Graphite::Key_KeypadDecimal: return ImGuiKey_KeypadDecimal;
	case Graphite::Key_KeypadDivide: return ImGuiKey_KeypadDivide;
	case Graphite::Key_KeypadMultiply: return ImGuiKey_KeypadMultiply;
	case Graphite::Key_KeypadSubtract: return ImGuiKey_KeypadSubtract;
	case Graphite::Key_KeypadAdd: return ImGuiKey_KeypadAdd;
	case Graphite::Key_KeypadEnter: return ImGuiKey_KeypadEnter;
	case Graphite::Key_KeypadEqual: return ImGuiKey_KeypadEqual;
	case Graphite::Key_AppBack: return ImGuiKey_AppBack;
	case Graphite::Key_AppForward: return ImGuiKey_AppForward;
	case Graphite::Key_GamepadStart: return ImGuiKey_GamepadStart;
	case Graphite::Key_GamepadBack: return ImGuiKey_GamepadBack;
	case Graphite::Key_GamepadFaceLeft: return ImGuiKey_GamepadFaceLeft;
	case Graphite::Key_GamepadFaceRight: return ImGuiKey_GamepadFaceRight;
	case Graphite::Key_GamepadFaceUp: return ImGuiKey_GamepadFaceUp;
	case Graphite::Key_GamepadFaceDown: return ImGuiKey_GamepadFaceDown;
	case Graphite::Key_GamepadDpadLeft: return ImGuiKey_GamepadDpadLeft;
	case Graphite::Key_GamepadDpadRight: return ImGuiKey_GamepadDpadRight;
	case Graphite::Key_GamepadDpadUp: return ImGuiKey_GamepadDpadUp;
	case Graphite::Key_GamepadDpadDown: return ImGuiKey_GamepadDpadDown;
	case Graphite::Key_GamepadL1: return ImGuiKey_GamepadL1;
	case Graphite::Key_GamepadR1: return ImGuiKey_GamepadR1;
	case Graphite::Key_GamepadL2: return ImGuiKey_GamepadL2;
	case Graphite::Key_GamepadR2: return ImGuiKey_GamepadR2;
	case Graphite::Key_GamepadL3: return ImGuiKey_GamepadL3;
	case Graphite::Key_GamepadR3: return ImGuiKey_GamepadR3;
	case Graphite::Key_GamepadLStickLeft: return ImGuiKey_GamepadLStickLeft;
	case Graphite::Key_GamepadLStickRight: return ImGuiKey_GamepadLStickRight;
	case Graphite::Key_GamepadLStickUp: return ImGuiKey_GamepadLStickUp;
	case Graphite::Key_GamepadLStickDown: return ImGuiKey_GamepadLStickDown;
	case Graphite::Key_GamepadRStickLeft: return ImGuiKey_GamepadRStickLeft;
	case Graphite::Key_GamepadRStickRight: return ImGuiKey_GamepadRStickRight;
	case Graphite::Key_GamepadRStickUp: return ImGuiKey_GamepadRStickUp;
	case Graphite::Key_GamepadRStickDown: return ImGuiKey_GamepadRStickDown;
	case Graphite::Key_MouseLeft: return ImGuiKey_MouseLeft;
	case Graphite::Key_MouseRight: return ImGuiKey_MouseRight;
	case Graphite::Key_MouseMiddle: return ImGuiKey_MouseMiddle;
	case Graphite::Key_MouseX1: return ImGuiKey_MouseX1;
	case Graphite::Key_MouseX2: return ImGuiKey_MouseX2;
	case Graphite::Key_MouseWheelX: return ImGuiKey_MouseWheelX;
	case Graphite::Key_MouseWheelY: return ImGuiKey_MouseWheelY;
	}
}

static uint32_t GraphiteMouseButtonToImGuiMouseIndex(Graphite::MouseButton mouseButton)
{
	switch (mouseButton)
	{
	case Graphite::MouseButton_Left: return 0;
	case Graphite::MouseButton_Middle: return 1;
	case Graphite::MouseButton_Right: return 2;
	default: return static_cast<uint32_t>(-1);
	}
}


namespace Graphite
{

	static bool OnKeyPressed(KeyPressedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent(GraphiteKeyToImGuiKey(event.GetKey()), true);

		// If it is a printable character
		if (char ascii = event.GetKeyAsASCII())
		{
			io.AddInputCharacter(ascii);
		}

		return io.WantCaptureKeyboard;
	}

	static bool OnKeyReleased(KeyReleasedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddKeyEvent(GraphiteKeyToImGuiKey(event.GetKey()), false);

		return io.WantCaptureKeyboard;
	}

	static bool OnMouseMoved(MouseMovedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMousePosEvent(static_cast<float>(event.GetMouseX()), static_cast<float>(event.GetMouseY()));

		return io.WantCaptureMouse;
	}

	static bool OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(GraphiteMouseButtonToImGuiMouseIndex(event.GetMouseButton()), true);

		return io.WantCaptureMouse;
	}

	static bool OnMouseButtonReleased(MouseButtonReleasedEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseButtonEvent(GraphiteMouseButtonToImGuiMouseIndex(event.GetMouseButton()), false);

		return io.WantCaptureMouse;
	}

	static bool OnMouseScrolled(MouseScrolledEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.AddMouseWheelEvent(0, static_cast<float>(event.GetScroll()));

		return io.WantCaptureMouse;
	}

	static bool OnWindowResize(WindowResizeEvent& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(static_cast<float>(event.GetWidth()), static_cast<float>(event.GetHeight()));

		return false;
	}


	void ImGuiLayer::OnAttach()
	{
		// Initialize ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Set up config

		// Set up style

		// Set up IO
		ImGuiIO& io = ImGui::GetIO();

		auto window = g_Application->GetWindow();
		io.DisplaySize = ImVec2(static_cast<float>(window->GetWidth()), static_cast<float>(window->GetHeight()));

		// Setup rendering backend
		GraphicsContext* graphicsContext = g_Application->GetGraphicsContext();

		m_ImGuiResources = graphicsContext->AllocateStaticDescriptors(1);
		GRAPHITE_ASSERT(m_ImGuiResources.IsValid(), "Failed to allocate ImGui resources.");

		m_Backend = g_Application->GetPlatform()->GetImGuiBackend();
		m_Backend->Init(*graphicsContext, m_ImGuiResources);

		// To capture all ImGui usage between updates of this layer
		NewFrame();
	}

	void ImGuiLayer::OnDetach()
	{
		m_Backend->Destroy();
		m_ImGuiResources.Free();

		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnRender()
	{
		// Render ImGui
		// This will also end the frame
		ImGui::Render();

		GraphicsContext* graphicsContext = g_Application->GetGraphicsContext();

		graphicsContext->BeginPass();
		{
			// Submit ImGui draw data to be rendered
			auto recordingContext = graphicsContext->AcquireRecordingContext();

			auto rtv = graphicsContext->GetBackBufferRenderTargetView();
			recordingContext->SetRenderTargets(1, rtv, std::nullopt);

			m_Backend->Render(ImGui::GetDrawData(), recordingContext);

			graphicsContext->CloseRecordingContext(recordingContext);
		}
		graphicsContext->EndPass();

		NewFrame();
	}


	void ImGuiLayer::OnEvent(Event& event)
	{
		// Propagate input to ImGui
		EventDispatcher dispatcher(event);

#define BIND_EVENT(func) std::bind(&(func), std::placeholders::_1)
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(OnKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT(OnKeyReleased));

		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT(OnMouseMoved));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT(OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT(OnMouseButtonReleased));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT(OnMouseScrolled));

		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(OnWindowResize));
	}

	void ImGuiLayer::NewFrame()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = g_Application->GetDeltaTime();

		m_Backend->NewFrame();
		ImGui::NewFrame();
	}

}
