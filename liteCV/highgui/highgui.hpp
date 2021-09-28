#pragma once
#ifndef LCV_HIGHGUI_HPP
#define LCV_HIGHGUI_HPP
#include <string>

#include "liteCV/core/matrix.hpp"
#include "flags.hpp"
#ifdef _WIN32
// Only support Windows yet.
#include "win32/winman_win32.hpp"
#else
#error "highgui cannot recognize platform"
#endif // _WIN32


namespace lcv
{
	void namedWindow(const std::string& title, int flags = WINDOW_AUTOSIZE)
	{
		auto w = WindowManager::instance()->get(title, flags);
		w->flags(flags);
		w->show();
	} // namedWindow

	void imshow(const std::string& title, const Matrix& mat)
	{
		auto win = WindowManager::instance()->get(title);
		win->show(mat);
	} // imshow

	void destroyWindow(const std::string& title)
	{
		WindowManager::instance()->release(title);
	} // destroyWindow

	void destroyAllWindows()
	{
		WindowManager::instance()->release_all();
	} // destroyAllWindows

	int waitKey(int delay = 0)
	{
		KMStatus km;
		WindowManager::event()->loop(delay);
		WindowManager::event()->status(km);
		return km.keyboad.keycode;
	} // waitKey
} // namespace lcv
#endif // LCV_HIGHGUI_HPP
