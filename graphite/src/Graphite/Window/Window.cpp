#include "pch.h"
#include "Window.h"

#include "Graphite/Core/Log.h"

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
            &m_WindowData); // Window user data - to retrieve event callback function in WinProc

        ShowWindow(m_HWND, SW_SHOWDEFAULT);

	}

	Window::~Window()
	{
		
	}


    bool Window::OnUpdate() const
    {
        // Main sample loop.
        MSG msg = {};
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                return false;
            }

            // Process any messages in the queue.
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return true;
    }


    // Window event callback handler
    LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        WindowData& windowData = *reinterpret_cast<WindowData*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        if (windowData.EventCallbackFn)
        {
            windowData.EventCallbackFn();
        }

        switch (message)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }

        // Handle any messages the switch statement didn't.
        return DefWindowProc(hWnd, message, wParam, lParam);
    }


}
