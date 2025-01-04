#include "graphite_pch.h"
#include "Window.h"

#include "Graphite/Core/Assert.h"
#include "Graphite/Core/Log.h"

#include "Graphite/Events/WindowEvent.h"
#include "Graphite/Events/KeyboardEvent.h"
#include "Graphite/Events/MouseEvent.h"


static Graphite::Key WindowsVirtualKeyToGraphiteKey(WPARAM wParam, LPARAM lParam)
{
    // There is no distinct VK_xxx for keypad enter, instead it is VK_RETURN + KF_EXTENDED.
    if ((wParam == VK_RETURN) && (HIWORD(lParam) & KF_EXTENDED))
        return Graphite::Key_KeypadEnter;

    switch (wParam)
    {
    case VK_TAB: return Graphite::Key_Tab;
    case VK_LEFT: return Graphite::Key_LeftArrow;
    case VK_RIGHT: return Graphite::Key_RightArrow;
    case VK_UP: return Graphite::Key_UpArrow;
    case VK_DOWN: return Graphite::Key_DownArrow;
    case VK_PRIOR: return Graphite::Key_PageUp;
    case VK_NEXT: return Graphite::Key_PageDown;
    case VK_HOME: return Graphite::Key_Home;
    case VK_END: return Graphite::Key_End;
    case VK_INSERT: return Graphite::Key_Insert;
    case VK_DELETE: return Graphite::Key_Delete;
    case VK_BACK: return Graphite::Key_Backspace;
    case VK_SPACE: return Graphite::Key_Space;
    case VK_RETURN: return Graphite::Key_Enter;
    case VK_ESCAPE: return Graphite::Key_Escape;
    case VK_OEM_7: return Graphite::Key_Apostrophe;
    case VK_OEM_COMMA: return Graphite::Key_Comma;
    case VK_OEM_MINUS: return Graphite::Key_Minus;
    case VK_OEM_PERIOD: return Graphite::Key_Period;
    case VK_OEM_2: return Graphite::Key_Slash;
    case VK_OEM_1: return Graphite::Key_Semicolon;
    case VK_OEM_PLUS: return Graphite::Key_Equal;
    case VK_OEM_4: return Graphite::Key_LeftBracket;
    case VK_OEM_5: return Graphite::Key_Backslash;
    case VK_OEM_6: return Graphite::Key_RightBracket;
    case VK_OEM_3: return Graphite::Key_GraveAccent;
    case VK_CAPITAL: return Graphite::Key_CapsLock;
    case VK_SCROLL: return Graphite::Key_ScrollLock;
    case VK_NUMLOCK: return Graphite::Key_NumLock;
    case VK_SNAPSHOT: return Graphite::Key_PrintScreen;
    case VK_PAUSE: return Graphite::Key_Pause;
    case VK_NUMPAD0: return Graphite::Key_Keypad0;
    case VK_NUMPAD1: return Graphite::Key_Keypad1;
    case VK_NUMPAD2: return Graphite::Key_Keypad2;
    case VK_NUMPAD3: return Graphite::Key_Keypad3;
    case VK_NUMPAD4: return Graphite::Key_Keypad4;
    case VK_NUMPAD5: return Graphite::Key_Keypad5;
    case VK_NUMPAD6: return Graphite::Key_Keypad6;
    case VK_NUMPAD7: return Graphite::Key_Keypad7;
    case VK_NUMPAD8: return Graphite::Key_Keypad8;
    case VK_NUMPAD9: return Graphite::Key_Keypad9;
    case VK_DECIMAL: return Graphite::Key_KeypadDecimal;
    case VK_DIVIDE: return Graphite::Key_KeypadDivide;
    case VK_MULTIPLY: return Graphite::Key_KeypadMultiply;
    case VK_SUBTRACT: return Graphite::Key_KeypadSubtract;
    case VK_ADD: return Graphite::Key_KeypadAdd;
    case VK_LSHIFT: return Graphite::Key_LeftShift;
    case VK_LCONTROL: return Graphite::Key_LeftCtrl;
    case VK_LMENU: return Graphite::Key_LeftAlt;
    case VK_LWIN: return Graphite::Key_LeftSuper;
    case VK_RSHIFT: return Graphite::Key_RightShift;
    case VK_RCONTROL: return Graphite::Key_RightCtrl;
    case VK_RMENU: return Graphite::Key_RightAlt;
    case VK_RWIN: return Graphite::Key_RightSuper;
    case VK_APPS: return Graphite::Key_Menu;
    case '0': return Graphite::Key_0;
    case '1': return Graphite::Key_1;
    case '2': return Graphite::Key_2;
    case '3': return Graphite::Key_3;
    case '4': return Graphite::Key_4;
    case '5': return Graphite::Key_5;
    case '6': return Graphite::Key_6;
    case '7': return Graphite::Key_7;
    case '8': return Graphite::Key_8;
    case '9': return Graphite::Key_9;
    case 'A': return Graphite::Key_A;
    case 'B': return Graphite::Key_B;
    case 'C': return Graphite::Key_C;
    case 'D': return Graphite::Key_D;
    case 'E': return Graphite::Key_E;
    case 'F': return Graphite::Key_F;
    case 'G': return Graphite::Key_G;
    case 'H': return Graphite::Key_H;
    case 'I': return Graphite::Key_I;
    case 'J': return Graphite::Key_J;
    case 'K': return Graphite::Key_K;
    case 'L': return Graphite::Key_L;
    case 'M': return Graphite::Key_M;
    case 'N': return Graphite::Key_N;
    case 'O': return Graphite::Key_O;
    case 'P': return Graphite::Key_P;
    case 'Q': return Graphite::Key_Q;
    case 'R': return Graphite::Key_R;
    case 'S': return Graphite::Key_S;
    case 'T': return Graphite::Key_T;
    case 'U': return Graphite::Key_U;
    case 'V': return Graphite::Key_V;
    case 'W': return Graphite::Key_W;
    case 'X': return Graphite::Key_X;
    case 'Y': return Graphite::Key_Y;
    case 'Z': return Graphite::Key_Z;
    case VK_F1: return Graphite::Key_F1;
    case VK_F2: return Graphite::Key_F2;
    case VK_F3: return Graphite::Key_F3;
    case VK_F4: return Graphite::Key_F4;
    case VK_F5: return Graphite::Key_F5;
    case VK_F6: return Graphite::Key_F6;
    case VK_F7: return Graphite::Key_F7;
    case VK_F8: return Graphite::Key_F8;
    case VK_F9: return Graphite::Key_F9;
    case VK_F10: return Graphite::Key_F10;
    case VK_F11: return Graphite::Key_F11;
    case VK_F12: return Graphite::Key_F12;
    case VK_F13: return Graphite::Key_F13;
    case VK_F14: return Graphite::Key_F14;
    case VK_F15: return Graphite::Key_F15;
    case VK_F16: return Graphite::Key_F16;
    case VK_F17: return Graphite::Key_F17;
    case VK_F18: return Graphite::Key_F18;
    case VK_F19: return Graphite::Key_F19;
    case VK_F20: return Graphite::Key_F20;
    case VK_F21: return Graphite::Key_F21;
    case VK_F22: return Graphite::Key_F22;
    case VK_F23: return Graphite::Key_F23;
    case VK_F24: return Graphite::Key_F24;
    case VK_BROWSER_BACK: return Graphite::Key_AppBack;
    case VK_BROWSER_FORWARD: return Graphite::Key_AppForward;
    default: return Graphite::Key_None;
    }
}

static bool IsVkDown(int vk)
{
    return (::GetKeyState(vk) & 0x8000) != 0;
}


namespace Graphite
{

    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	Window::Window(const GraphiteWindowDesc& windowDesc)
	{
        GRAPHITE_LOG_INFO("Creating window at resolution: ({}x{})", windowDesc.Width, windowDesc.Height);

        const HINSTANCE hInstance = GetModuleHandle(nullptr);

        // Initialize the window class.
        WNDCLASSEX windowClass = { 0 };
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = Window::WindowProc;
        windowClass.hInstance = hInstance;
        windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
        windowClass.lpszClassName = windowDesc.WindowName.c_str();
        RegisterClassEx(&windowClass);

        m_WindowData.Width = windowDesc.Width;
        m_WindowData.Height = windowDesc.Height;
        m_WindowData.PreviousWidth = windowDesc.Width;
        m_WindowData.PreviousHeight = windowDesc.Height;

        RECT windowRect = { 0, 0, static_cast<LONG>(m_WindowData.Width), static_cast<LONG>(m_WindowData.Height) };
        AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

        // Create the window and store a handle to it.
        m_HWND = CreateWindow(
            windowClass.lpszClassName,
            windowDesc.WindowTitle.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            nullptr,        // We have no parent window.
            nullptr,        // We aren't using menus.
            hInstance,
            nullptr);

		// Window user data - to retrieve event callback function in WinProc
        SetWindowLongPtr(m_HWND, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&m_WindowData));

        ShowWindow(m_HWND, SW_SHOWDEFAULT);
	}

	Window::~Window()
	{
		
	}

    Viewport Window::GetDefaultViewport() const
    {
        return {
            0.0f,
        	0.0f,
            static_cast<float>(m_WindowData.Width),
            static_cast<float>(m_WindowData.Height),
            0.0f,
        	1.0f
        };
    }

    Rectangle Window::GetDefaultRectangle() const
    {
        return {
            0,
            0,
            static_cast<int32_t>(m_WindowData.Width),
            static_cast<int32_t>(m_WindowData.Height),
        };
    }


    void Window::SetEventCallback(const EventCallbackFn& eventCallback)
    {
        m_WindowData.EventCallbackFn = eventCallback;
    }



    void Window::BufferMessages()
    {
        // Main sample loop.
        MSG msg = {};
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // Process any messages in the queue.
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void Window::SubmitKeyEvent(WindowData& windowData, Key key, bool down, char ascii)
	{
		if (down)
		{
            KeyPressedEvent event(key, ascii);
            windowData.EventCallbackFn(event);
		}
        else
        {
            KeyReleasedEvent event(key, ascii);
            windowData.EventCallbackFn(event);
        }
	}

    // Window event callback handler
    LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        LONG_PTR userData = GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (!userData)
        {
            // No user data - cannot handle messages
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        WindowData& windowData = *reinterpret_cast<WindowData*>(userData);


        switch (message)
        {
        default:
            // Handle any messages the switch statement doesn't
            return DefWindowProc(hWnd, message, wParam, lParam);

            // Window events
        case WM_DESTROY:
        {
            WindowCloseEvent event;
            windowData.EventCallbackFn(event);
            return 0;
	    }
            // Keyboard events
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            bool down = (message == WM_KEYDOWN || message == WM_SYSKEYDOWN);
            auto key = WindowsVirtualKeyToGraphiteKey(wParam, lParam);
            const int vk = static_cast<int>(wParam);
            const int scancode = static_cast<int>(LOBYTE(HIWORD(lParam)));

			if (key != Key_None)
            {
                // Try and get ASCII representation of character
                char ascii = 0;
                {
                    BYTE keyboardState[256];
                    WORD w;
                    bool success = GetKeyboardState(keyboardState);
                    int charsWritten = ToAscii(vk, scancode, success ? keyboardState : nullptr, &w, 0);

                    // Don't handle characters that are not represented by a single ASCII character
                    if (charsWritten == 1)
                    {
                        ascii = LOBYTE(w);
                    }
                }

                SubmitKeyEvent(windowData, key, down, ascii);
			}
            
            // TODO: This does end up sending duplicate events. It would still be correct for input polling purposes
            // Acting on event inputs may need to be handled in a different way than through the core events system
            // - i.e. the input polling system will be able to recognise when an event is a duplicate
            if (vk == VK_SHIFT)
            {
                if (IsVkDown(VK_LSHIFT) == down)
                {
                    SubmitKeyEvent(windowData, Key_LeftShift, down);
                }
                if (IsVkDown(VK_RSHIFT) == down)
                {
                    SubmitKeyEvent(windowData, Key_RightShift, down);
                }
            }
            else if (vk == VK_CONTROL)
            {
                if (IsVkDown(VK_LCONTROL) == down)
                {
                    SubmitKeyEvent(windowData, Key_LeftCtrl, down);
                }
                if (IsVkDown(VK_RCONTROL) == down)
                {
                    SubmitKeyEvent(windowData, Key_RightCtrl, down);
                }
            }
            else if (vk == VK_MENU)
            {
                if (IsVkDown(VK_LMENU) == down)
                {
                    SubmitKeyEvent(windowData, Key_LeftAlt, down);
                }
                if (IsVkDown(VK_RMENU) == down)
                {
                    SubmitKeyEvent(windowData, Key_RightAlt, down);
                }
            }

            return 0;
        }
            // Mouse events
        case WM_MOUSEMOVE:
	    {
            MouseMovedEvent event(static_cast<uint32_t>(LOWORD(lParam)), static_cast<uint32_t>(HIWORD(lParam)));
            windowData.EventCallbackFn(event);
            return 0;
	    }
        case WM_LBUTTONDOWN:
	    {
            MouseButtonPressedEvent event(MouseButton_Left);
            windowData.EventCallbackFn(event);
            return 0;
	    }
        case WM_MBUTTONDOWN:
	    {
            MouseButtonPressedEvent event(MouseButton_Middle);
            windowData.EventCallbackFn(event);
            return 0;
	    }
        case WM_RBUTTONDOWN:
	    {
            MouseButtonPressedEvent event(MouseButton_Right);
            windowData.EventCallbackFn(event);
            return 0;
	    }
        case WM_LBUTTONUP:
	    {
            MouseButtonReleasedEvent event(MouseButton_Left);
            windowData.EventCallbackFn(event);
            return 0;
	    }
        case WM_MBUTTONUP:
	    {
            MouseButtonReleasedEvent event(MouseButton_Middle);
            windowData.EventCallbackFn(event);
            return 0;
	    }
        case WM_RBUTTONUP:
	    {
            MouseButtonReleasedEvent event(MouseButton_Right);
            windowData.EventCallbackFn(event);
            return 0;
	    }
        case WM_MOUSEWHEEL:
	    {
            MouseScrolledEvent event(static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / static_cast<float>(WHEEL_DELTA));
            windowData.EventCallbackFn(event);
            return 0;
	    }
        // Resize events
        case WM_ENTERSIZEMOVE:
	    {
            windowData.Resizing = true;
            return 0;
	    }
        case WM_SIZE:
	    {
            // Called when the window is being actively resized.
            // We don't want to submit resize events in this stage - we want to wait until resizing is complete
            windowData.Width = LOWORD(lParam);
            windowData.Height = HIWORD(lParam);

            if (wParam == SIZE_MAXIMIZED || (wParam == SIZE_RESTORED && !windowData.Resizing))
            {
				// An enter/exit size move isn't sent in this situation
                // so the resize event will have to be submitted at this point
                WindowResize(windowData);
            }
            return 0;
	    }
        case WM_EXITSIZEMOVE:
	    {
            windowData.Resizing = false;

            WindowResize(windowData);
            return 0;
	    }
        }
    }

    void Window::WindowResize(WindowData& windowData)
    {
        if (windowData.Width != windowData.PreviousWidth || windowData.Height != windowData.PreviousHeight)
        {
            windowData.PreviousWidth = windowData.Width;
            windowData.PreviousHeight = windowData.Height;

            WindowResizeEvent event(windowData.Width, windowData.Height);
            windowData.EventCallbackFn(event);
        }
    }


}
