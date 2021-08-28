#pragma once
#ifndef LCV_HIGHGUI_WINMAN
#define LCV_HIGHGUI_WINMAN
#include <memory>
#include <string>
#include <list>
#include <map>

#include "liteCV/core/lcvdef.hpp"
#include "liteCV/core/matrix.hpp"

#include "flags.hpp"


namespace lcv
{
    struct KMStatus
    {
        struct
        {
            int status;
            int scancode;
            int keycode;
        } keyboad;

        struct
        {
            int status;
            int button;
            struct
            {
                int x;
                int y;
            } pos;
        } mouse;

        KMStatus()
        {
            memset(&keyboad, 0, sizeof(keyboad));
            memset(&mouse, 0, sizeof(mouse));
        }
    };

    class IEvent : Noncopyable
    {
    public:
        virtual int quit(int exitcode) = 0;
        virtual int loop(int delay) = 0;
        virtual void status(KMStatus& km) = 0;
    }; // class IEvent

    class IWindow : Noncopyable
    {
    protected:
        std::string title;

    protected:
        IWindow() = default;

    public:
        IWindow(const std::string& title)
            : title(title) {}

    public:
        const std::string& get_title() const
        {
            return title;
        }

    public:
        virtual void reuse() = 0;
        virtual void flags(int flags) = 0;
        virtual void show() = 0;
        virtual void show(const Matrix& mat) = 0;
        virtual void close() = 0;
        virtual void resize(int width, int height) = 0;
        virtual void move(int x, int y) = 0;
    }; // class IWindow

    template<typename _Event, typename _Window>
    class WindowManager_
    {
    public:
        static _Event* event()
        {
            static _Event e;
            return &e;
        }

        static WindowManager_* instance()
        {
            static WindowManager_ wm;
            return &wm;
        }

    private:
        WindowManager_() = default;

        ~WindowManager_()
        {
            release_all();
        }

    private:
        std::list<_Window*> MyWindows;
        std::map<std::string, typename std::list<_Window*>::iterator> MyWindowsByTitle;

    public:
        bool inline empty() const
        {
            return MyWindows.size() == 0;
        }

    public:
        _Window* get(const std::string& title, int flags = WINDOW_AUTOSIZE)
        {
            _Window* w;

            // Find window which has same title from window map.
            auto pair = MyWindowsByTitle.find(title);
            if (pair != MyWindowsByTitle.cend())
            {
                // Reuse window
                w = *pair->second;
            }
            else
            {
                // Create new one
                w = new _Window(title);
                MyWindows.push_back(w);
                MyWindowsByTitle[title] = (--MyWindows.end());
            }

            // Call reuse() before returning
            w->reuse();
            return w;
        }

        void release(const std::string& title)
        {
            // Find window which has same title from window map.
            auto pair = MyWindowsByTitle.find(title);
            if (pair != MyWindowsByTitle.cend())
            {
                // Release window
                auto it = pair->second;
                _Window* w = *it;

                MyWindowsByTitle.erase(pair);
                MyWindows.erase(it);
                delete w;
            }
        }

        void release_all()
        {
            for (auto it : MyWindows)
            {
                delete it;
            }

            MyWindows.clear();
            MyWindowsByTitle.clear();
        }
    }; // class WindowManager_
} // namespace lcv
#endif // LCV_HIGHGUI_WINMAN
