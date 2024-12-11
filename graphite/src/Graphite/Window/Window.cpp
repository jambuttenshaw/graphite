#include "pch.h"
#include "Window.h"

#include "Graphite/Core/Assert.h"
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
            nullptr);
        GRAPHITE_LOG_INFO("{}", reinterpret_cast<LONG_PTR>(m_HWND));
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
        LONG_PTR userData = GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (!userData)
        {
            // No user data - cannot handle messages
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        WindowData& windowData = *reinterpret_cast<WindowData*>(userData);


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
