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

        m_Width = windowDesc.Width;
        m_Height = windowDesc.Height;
        m_WindowRect = { 0, 0, static_cast<LONG>(m_Width), static_cast<LONG>(m_Height) };
        AdjustWindowRect(&m_WindowRect, WS_OVERLAPPEDWINDOW, FALSE);

        // Create the window and store a handle to it.
        m_HWND = CreateWindow(
            windowClass.lpszClassName,
            windowDesc.WindowTitle.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            m_WindowRect.right - m_WindowRect.left,
            m_WindowRect.bottom - m_WindowRect.top,
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



    void Window::BufferMessageQueue() const
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
            KeyPressedEvent event(static_cast<int32_t>(wParam));
            windowData.EventCallbackFn(event);
            return 0;
	        }
        case WM_KEYUP:
        {
            KeyReleasedEvent event(static_cast<int32_t>(wParam));
            windowData.EventCallbackFn(event);
            return 0;
        }
            // Mouse events
        case WM_MOUSEMOVE:
            return 0;
        case WM_LBUTTONDOWN:
            return 0;
        case WM_MBUTTONDOWN:
            return 0;
        case WM_RBUTTONDOWN:
            return 0;
        case WM_LBUTTONUP:
            return 0;
        case WM_MBUTTONUP:
            return 0;
        case WM_RBUTTONUP:
            return 0;
        case WM_MOUSEWHEEL:
            return 0;
        }

        GRAPHITE_ASSERT(false, "Missing return statement within switch case.");
    }


}
