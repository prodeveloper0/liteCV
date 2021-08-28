#pragma once
#ifndef LCV_HIGHGUI_WIN32_WINMAN_HPP
#define LCV_HIGHGUI_WIN32_WINMAN_HPP
#include <string>
#include <map>
#include <list>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "liteCV/core/lcvdef.hpp"
#include "liteCV/core/matrix.hpp"

#include "../flags.hpp"
#include "../winman.hpp"
#include "dib.hpp"


namespace lcv
{
    LRESULT CALLBACK GlobalWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    class EventWin32 : public IEvent
    {
    private:
        KMStatus MyKm;

    public:
        int quit(int exitcode) final
        {
            ::PostQuitMessage(exitcode);
            return exitcode;
        }

        int loop(int delay) final
        {
            MSG msg;
            ZeroMemory(&msg, sizeof(msg));
            ZeroMemory(&MyKm, sizeof(KMStatus));

            while (GetMessageA(&msg, NULL, 0, 0))
            {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);

                switch (msg.message)
                {
                case WM_CHAR:
                    MyKm.keyboad.keycode = (int)msg.wParam;
                    break;
                }
            }

            return msg.wParam;
        }

        void status(KMStatus& km) final
        {
            km = MyKm;
        }
    }; // class EventWin32

    class WindowWin32 : public IWindow
    {
    private:
        class WindowClass
        {
        private:
            WNDCLASSA wc;

        public:
            const char* name() const
            {
                static const char* classname = "liteCV_WindowWin32";
                return classname;
            }

            WNDCLASSA& native()
            {
                return wc;
            }

            const WNDCLASSA& native() const
            {
                return wc;
            }

            WNDCLASSA* native_ptr()
            {
                return &wc;
            }

            const WNDCLASSA* native_ptr() const
            {
                return &wc;
            }

        public:
            WindowClass()
            {
                wc.lpszClassName = name();
                wc.lpfnWndProc = GlobalWndProc;
                wc.hCursor = LoadCursorA(NULL, (LPCSTR)IDC_ARROW);
                wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
                RegisterClassA(&wc);
            }

            ~WindowClass()
            {
                ::UnregisterClassA(name(), NULL);
            }
        };

        static WindowClass* wndcls()
        {
            static WindowClass wc;
            return &wc;
        }

    private:
        HWND hwnd;
        DIB dib;

    public:
        using IWindow::IWindow;

        ~WindowWin32()
        {
            Destroy();
        }

        WindowWin32(WindowWin32&& another) noexcept
        {
            Destroy();
            hwnd = another.hwnd;
            another.hwnd = 0;
        }

        WindowWin32& operator=(WindowWin32&& another) noexcept
        {
            Destroy();
            hwnd = another.hwnd;
            another.hwnd = 0;
            return *this;
        }

    public:
        void Destroy()
        {
            if (hwnd != 0)
            {
                ::DestroyWindow(hwnd);
                hwnd = 0;
            }
        }

    public:
        DIB& get_dib()
        {
            return dib;
        }

        const DIB& get_dib() const
        {
            return dib;
        }

    public:
        void reuse() final
        {
            // Window is already created.
            if (hwnd != 0)
            {
                return;
            }

            // Create new window.
            hwnd = CreateWindowExA(0, wndcls()->name(), get_title().c_str(),
                WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
                CW_USEDEFAULT, CW_USEDEFAULT, 320, 240,
                NULL, NULL, NULL, NULL);

            assert(hwnd != 0);
            if (hwnd != 0)
            {
                // Store current instance to HWND.
                SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)this);
            }
        }

        void flags(int flags) final
        {
            DWORD dwStyle = WS_OVERLAPPEDWINDOW;
            if ((flags & WINDOW_AUTOSIZE) != 0)
            {
                dwStyle = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;
            }

            ::SetWindowLongA(hwnd, GWL_STYLE, dwStyle);
        }

        void show() final
        {
            ::ShowWindow(hwnd, SW_SHOWNORMAL);
        }

        void show(const Matrix& mat) final
        {
            dib = DIB::FromMatrix(mat);
            show();
            resize(mat.cols, mat.rows);

            RECT rect;
            ::GetClientRect(hwnd, &rect);
            ::InvalidateRect(hwnd, &rect, FALSE);
        }

        void close() final
        {
            ::CloseWindow(hwnd);
        }

        void resize(int width, int height) final
        {
            RECT rect = { 0, 0, width, height };
            ::AdjustWindowRect(&rect, ::GetWindowLongA(hwnd, GWL_STYLE), FALSE);
            ::SetWindowPos(hwnd, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);
        }

        void move(int x, int y) final
        {
            ::SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }
    }; // class WindowWin32

    LRESULT CALLBACK GlobalWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        static std::atomic<unsigned long long> create_count(0ull);
        WindowWin32* pw = (WindowWin32*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

        PAINTSTRUCT ps;
        HDC hdc;

        switch (uMsg)
        {
        case WM_CREATE:
            // Increase window creation count
            ++create_count;
            break;

        case WM_CLOSE:
            pw->Destroy();
            return 0;

        case WM_DESTROY:
            // Decrease window creation count
            if ((--create_count) == 0)
            {
                // If window creation count is zero, quit message loop finally
                ::PostQuitMessage(0);
            }
            return 0;

        case WM_CHAR:
            // Quit message loop first
            ::PostQuitMessage(0);
            break;

        case WM_PAINT:
            // Draw image
            if (!pw->get_dib().IsEmpty())
            {
                hdc = ::BeginPaint(hwnd, &ps);
                pw->get_dib().Draw(hwnd, hdc);
                ::EndPaint(hwnd, &ps);
            }
            break;

        case WM_GETMINMAXINFO:
            // Set minimum window size
            ((LPMINMAXINFO)lParam)->ptMinTrackSize.x = 256;
            ((LPMINMAXINFO)lParam)->ptMinTrackSize.y = 128;
            break;
        }

        return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    } 

    using Event = EventWin32;
    using Window = WindowWin32;
    using WindowManager = WindowManager_<Event, Window>;
} // namespace lcv
#endif // LCV_HIGHGUI_WIN32_WINMAN_HPP
