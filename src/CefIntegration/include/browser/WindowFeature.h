
#ifndef WINDOW_FEATURE_H_
#define WINDOW_FEATURE_H_

#include <string>
#include <vector>
#include "Windows.h"

namespace cefIntegration{

enum BehaviorIfSCClose{
	BSC_CLOSE_WINDOW = 0,
	BSC_HIDE,
	BSC_QUIT,
	BSC_MINIMIZE,
};

struct WindowFeature {
	WindowFeature() : isChild(false), left(0), top(0), width(0), height(0), isMaximize(false), centerScreen(false), refreshIfExist(false),
		modal(false), resizable(false), minWidth(0), minHeight(0), hasTaskBarIcon(true), isTop(false), 
		isShow(true), behaviorIfSCClose(BSC_CLOSE_WINDOW), enableDrag(false), leftBorder(0), topBorder(0),
		rightBorder(0), bottomBorder(0), isLayered(false){
	}

	bool isChild;
	std::wstring title;
	int left;
	int top;
	int width;
	int height;
	bool isMaximize;
	bool centerScreen;
	bool refreshIfExist;
	bool modal;
	bool resizable;
	int minWidth;
	int minHeight;
	bool hasTaskBarIcon;
	bool isTop;
	bool isShow;
	BehaviorIfSCClose behaviorIfSCClose;
	bool enableDrag;
	int leftBorder;
	int topBorder;
	int rightBorder;
	int bottomBorder;
	bool isLayered;  //是否要创建透明窗口;
	std::vector<RECT> captions;
};

struct MenuFeature{
	int id;
	std::wstring icon;
	std::wstring label;
};

}
#endif