#include "pch.h"
#include "Window.h"

#include "Graphite/Core/Assert.h"
#include "Graphite/Core/Log.h"

#include "Graphite/Events/WindowEvent.h"
#include "Graphite/Events/KeyboardEvent.h"
#include "Graphite/Events/MouseEvent.h"


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
	        {
            KeyPressedEvent event(static_cast<KeyCode>(wParam));
            windowData.EventCallbackFn(event);
            return 0;
	        }
        case WM_KEYUP:
        {
            KeyReleasedEvent event(static_cast<KeyCode>(wParam));
            windowData.EventCallbackFn(event);
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
            MouseButtonPressedEvent event(MouseButtons::KEY_LBUTTON);
            windowData.EventCallbackFn(event);
            return 0;
	    }
        case WM_MBUTTONDOWN:
	    {
            MouseButtonPressedEvent event(MouseButtons::KEY_MBUTTON);
            windowData.EventCallbackFn(event);
            return 0;
	    }
        case WM_RBUTTONDOWN:
	    {
            MouseButtonPressedEvent event(MouseButtons::KEY_RBUTTON);
            windowData.EventCallbackFn(event);
            return 0;
	    }
        case WM_LBUTTONUP:
	    {
            MouseButtonReleasedEvent event(MouseButtons::KEY_LBUTTON);
            windowData.EventCallbackFn(event);
            return 0;
	    }
        case WM_MBUTTONUP:
	    {
            MouseButtonReleasedEvent event(MouseButtons::KEY_MBUTTON);
            windowData.EventCallbackFn(event);
            return 0;
	    }
        case WM_RBUTTONUP:
	    {
            MouseButtonReleasedEvent event(MouseButtons::KEY_RBUTTON);
            windowData.EventCallbackFn(event);
            return 0;
	    }
        case WM_MOUSEWHEEL:
	    {
            MouseScrolledEvent event(static_cast<int32_t>(GET_WHEEL_DELTA_WPARAM(wParam)));
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

        GRAPHITE_ASSERT(false, "Missing return statement within switch case - return 0 to indicate the message has been handled.");
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
