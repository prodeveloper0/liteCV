#pragma once
#ifndef HIGHGUI_FLAGS_HPP
#define HIGHGUI_FLAGS_HPP
namespace lcv
{
	enum WindowFlags
	{
		WINDOW_NORMAL = 0x00000000,
		WINDOW_AUTOSIZE = 0x00000001,
		WINDOW_FREERATIO = 0x00000100,
		WINDOW_KEEPRATIO = 0x00000000,
	}; // enum WindowFlags
} // namespace lcv
#endif // HIGHGUI_FLAGS_HPP
