#include "WindowExtension.h"
#include "CefJSConversion.h"
#include "CefClient.h"
#include "utils/Dialog.h"
#include "LogManager.h"

#include <sstream>

namespace cefIntegration {

using namespace uc;

WindowExtension::WindowExtension(CefClient* cefClient) : _cefClient(cefClient) {
}

bool WindowExtension::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
	CefProcessId source_process, CefRefPtr<CefProcessMessage> message) {
	if (message->GetName() == "JSCommond") {

		CefRefPtr<CefListValue> arguments = message->GetArgumentList();
		int argLen = arguments->GetSize();
	
		bool hasCallback = false;
		int res = CEF_SUCCESS;
	
		//检查message参数：request id，hasCallback, module，method name
		if ((argLen < 4) || (arguments->GetType(0) != VTYPE_INT) || 
			(arguments->GetType(1) != VTYPE_BOOL) || (arguments->GetType(2) != VTYPE_STRING)||
			(arguments->GetType(3) != VTYPE_STRING)) {
			res = CEF_ARGUMENT_ERROR;

			UC_LOG(ERROR) << "JSCommond message argument error, argLen: "
				<< argLen << ", type0: " << arguments->GetType(0) << ", type1: " << arguments->GetType(1) << ", type2: "
				<< arguments->GetType(2) << ", type3: " << arguments->GetType(3) << ", hwnd: " << browser->GetHost()->GetWindowHandle();

		}
		else {
			int requestId = arguments->GetInt(0);
			bool hasCallback = arguments->GetBool(1);
			CefString module = arguments->GetString(2);
			CefString method = arguments->GetString(3);

			CefRefPtr<CefProcessMessage> response = CefProcessMessage::Create("InvokeCallback");
		    CefRefPtr<CefListValue> responseArgs = response->GetArgumentList();

		    if (module == L"Window") {
		    	WindowMethodProcess(browser, method, argLen, arguments, res,responseArgs);
		    }
			else if (module == L"App") {
				AppMethodProcess(browser, method, argLen, arguments, res,responseArgs);
		    }
			else if (module == L"Dialog") {
				hasCallback = false;
				DialogMethodProcess(browser, method, argLen, arguments, res,responseArgs, requestId);
		    }
			else {
				res = CEF_MODULE_NOTFOUND;
			}

			if (hasCallback) {
		 		responseArgs->SetInt(0, requestId);
            	responseArgs->SetInt(1, res);
          
            	browser->SendProcessMessage(PID_RENDERER, response);	
			}

			UC_LOG(INFO) << "CallNativeMethodAync result, " << "requestId: " << requestId << ", hasCallback: " << hasCallback
				<< ", module: " << module.ToString() << ", method: " << method.ToString() << ", res: " << res 
				<< ", hwnd: " << browser->GetHost()->GetWindowHandle();

		
		}
		return true;
	}

	return false;
}

void WindowExtension::DialogMethodProcess(CefRefPtr<CefBrowser> browser, CefString method, int argLen, 
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs, int requestId) {
	if (method == L"RunOpenDialog") {
		if ((argLen == 7)  && (arguments->GetType(4) == VTYPE_STRING) &&  
			(arguments->GetType(5) == VTYPE_LIST) && (arguments->GetType(6) == VTYPE_BOOL)){
			CefString title = arguments->GetString(4);
			
			std::map<std::wstring, std::wstring> filters;
			CefRefPtr<CefListValue> filterList = arguments->GetList(5);
			for (std::size_t i = 0; i < filterList->GetSize(); ++i) {
				if (filterList->GetType(i) == VTYPE_DICTIONARY) {
					CefRefPtr<CefDictionaryValue> dict = filterList->GetDictionary(i);
					filters[dict->GetString("desc").ToWString()] = dict->GetString("filter").ToWString();
				}
			}
			
			bool isMulti = arguments->GetBool(6);
			Dialog dialog;
			dialog.RunOpenDialog(browser, title, filters,isMulti, requestId);
		}
		else {
			res = CEF_ARGUMENT_ERROR;
		}
	}
	else if (method == L"RunSaveDialog") {
		if ((argLen == 7)  && (arguments->GetType(4) == VTYPE_STRING) &&  
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_LIST)){
			CefString title = arguments->GetString(4);
			CefString fileName = arguments->GetString(5);

			std::map<std::wstring, std::wstring> filters;
			CefRefPtr<CefListValue> filterList = arguments->GetList(6);
			for (std::size_t i = 0; i < filterList->GetSize(); ++i) {
				if (filterList->GetType(i) == VTYPE_DICTIONARY) {
					CefRefPtr<CefDictionaryValue> dict = filterList->GetDictionary(i);
					filters[dict->GetString("desc").ToWString()] = dict->GetString("filter").ToWString();
				}
			}

			Dialog dialog;
			dialog.RunSaveDialog(browser, title, fileName, filters, requestId);
		}
		else {
			res = CEF_ARGUMENT_ERROR;
		}
	}
	else {
		res = CEF_METHOD_NOTFOUND;
	}
}

void WindowExtension::AppMethodProcess(CefRefPtr<CefBrowser> browser, CefString method, int argLen, 
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs) {
	if (method == L"Quit") {
		_cefClient->Quit();
	}
	else if (method == L"DispatchGlobalEvent") {
		if ((argLen == 6)  && (arguments->GetType(4) == VTYPE_STRING) &&  
			(arguments->GetType(5) == VTYPE_DICTIONARY)){
				std::wstring name = arguments->GetString(4).ToWString();
				CefRefPtr<CefDictionaryValue> dict = arguments->GetDictionary(5);

				std::wstring data;
				CefJSConversion::DictionaryToString(dict, data);

				std::wstringstream jsScript;
				jsScript << L"var e = new CustomEvent('" << name
					<< "',{detail:" << "{data: " << data << "},});document.dispatchEvent(e);";
				_cefClient->ExecuteJSForAllBrowsers(jsScript.str());
			}
			else {
				res = CEF_ARGUMENT_ERROR;
			}
	}
	else {
		res = CEF_METHOD_NOTFOUND;
	}
}

void WindowExtension::WindowMethodProcess(CefRefPtr<CefBrowser> browser, CefString method, int argLen, 
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs) {
	if (method == L"Open") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_STRING) &&
			(arguments->GetType(5) == VTYPE_STRING) && (arguments->GetType(6) == VTYPE_DICTIONARY)
			&& (arguments->GetType(7) == VTYPE_INT)){
				std::wstring url = arguments->GetString(4).ToWString();
				std::wstring name = arguments->GetString(5).ToWString();
				CefRefPtr<CefDictionaryValue> dict = arguments->GetDictionary(6);
				AfterBehaviorOpenWindow afterBehavior = AfterBehaviorOpenWindow(arguments->GetInt(7));

				WindowFeature feature;
				ConvertDictionaryToWindowFeature(dict, feature);

				_cefClient->getWindowManager()->Open(browser, url, name, feature, afterBehavior);
			}
			else {
				res = CEF_ARGUMENT_ERROR;
			}
	}
	else if (method == L"Navigate") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_STRING)){
				std::wstring url = arguments->GetString(4).ToWString();
				_cefClient->getWindowManager()->Navigate(browser, url);
			}
			else {
				res = CEF_ARGUMENT_ERROR;
			}
	}
	else if (method == L"SetSize") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_INT)
			 && (arguments->GetType(5) == VTYPE_INT)){
				 int width = arguments->GetInt(4);
				 int height = arguments->GetInt(5);
				 _cefClient->getWindowManager()->SetSize(browser, width, height);
			}
			else {
				res = CEF_ARGUMENT_ERROR;
			}
	}
	else if (method == L"CenterWindow") {
		_cefClient->getWindowManager()->CenterWindow(browser);
	}
	else if (method == L"Minimize") {
		_cefClient->getWindowManager()->Minimize(browser);
	}
	else if (method == L"Maximize") {
		_cefClient->getWindowManager()->Maximize(browser);
	}
	else if (method == L"FullScreen") {
		_cefClient->getWindowManager()->FullScreen(browser);
	}
	else if (method == L"Restore") {
		_cefClient->getWindowManager()->Restore(browser);
	}
	else if (method == L"Hide") {
		_cefClient->getWindowManager()->Hide(browser);
	}
	else if (method == L"HideAll") {
		_cefClient->getWindowManager()->HideAll();
	}
	else if (method == L"Close") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_STRING)) {
			std::wstring name = arguments->GetString(4).ToWString();
			_cefClient->getWindowManager()->Close(browser, name);
		}
		
	}
	else if (method == L"Show") {
		_cefClient->getWindowManager()->Show(browser);
	}
	else if (method == L"SetCaptions") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_LIST)) {
				CefRefPtr<CefListValue> captions = arguments->GetList(4);

				std::vector<RECT> rect;
				ConverListToCaptions(captions, rect);

				_cefClient->getWindowManager()->SetCaptions(browser, rect);
			}
			else {
				res = CEF_ARGUMENT_ERROR;
			}
	}
	else if (method == L"SetTray") {
		if ((argLen >= 7) &&  (arguments->GetType(4) == VTYPE_STRING) &&
			 (arguments->GetType(5) == VTYPE_STRING)) {
			std::wstring title = arguments->GetString(4).ToWString();
			std::wstring image = arguments->GetString(5).ToWString();
			bool isFlash = arguments->GetBool(6);
			std::vector<MenuFeature> menuFeatures;
			if ( argLen == 8 && arguments->GetType(7) == VTYPE_DICTIONARY ) {
				CefRefPtr<CefDictionaryValue> featureDict = arguments->GetDictionary(7);
				if (featureDict->HasKey(L"menu") && featureDict->GetType(L"menu")==VTYPE_LIST) {
					CefRefPtr<CefListValue> menuList  = featureDict->GetList(L"menu");
					for (std::size_t i = 0; i < menuList->GetSize(); ++i) {
						if (menuList->GetType(i) == VTYPE_DICTIONARY) {
							MenuFeature feature;
							ConvertDictionaryToMenuFeature(menuList->GetDictionary(i), feature);
							menuFeatures.push_back(feature);
						}
					}
				}
			}		

			_cefClient->getWindowManager()->SetTray(browser, title, image, isFlash, menuFeatures);
		}
		else {
			res = CEF_ARGUMENT_ERROR;
		}
	}
    else if (method == L"FlashTray") {
        if ((argLen >= 6) && (arguments->GetType(5) == VTYPE_INT)) {
            bool isFlash = arguments->GetBool(4);
            int interval = arguments->GetInt(5);

            _cefClient->getWindowManager()->FlashTray(browser, isFlash, interval);
        }
        else {
            res = CEF_ARGUMENT_ERROR;
        }
    }
	else if (method == L"DisableDBClick") {
		_cefClient->getWindowManager()->DisableDBClick(browser);
	}
	else if (method == L"EnableDBClick") {
		_cefClient->getWindowManager()->EnableDBClick(browser);
	}
    else if (method == L"Move") {
        if ((argLen >= 6) && (arguments->GetType(4) == VTYPE_INT) && (arguments->GetType(5) == VTYPE_INT)) {
            _cefClient->getWindowManager()->Move(browser, arguments->GetInt(4), arguments->GetInt(5));
        }
        else {
            res = CEF_ARGUMENT_ERROR;
        }
    }
	else if (method == L"SetTrayBalloon") {
		if ((argLen == 8) && (arguments->GetType(4) == VTYPE_INT)
			&& (arguments->GetType(5) == VTYPE_STRING) &&
			(arguments->GetType(6) == VTYPE_STRING) &&
			(arguments->GetType(7) == VTYPE_INT)) {

			ToolTipIcon icon = ToolTipIcon(arguments->GetInt(4));
			std::wstring& title = arguments->GetString(5).ToWString();
			std::wstring& text = arguments->GetString(6).ToWString();
			int timeOut = arguments->GetInt(7);

			_cefClient->getWindowManager()->SetTrayBalloon(browser, icon, title, text, timeOut);
		}
		else {
			res = CEF_ARGUMENT_ERROR;
		}
	}
	else if (method == L"FlashWindow") {
		if ((argLen == 6) && (arguments->GetType(4) == VTYPE_INT)
			 && (arguments->GetType(5) == VTYPE_INT)) {
			UINT count =  arguments->GetInt(4);
			UINT timeout =  arguments->GetInt(5);

			_cefClient->getWindowManager()->FlashWindow(browser, count, timeout);
		}
		else {
			res = CEF_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SetIconWithNumber") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)) {
			int count = arguments->GetInt(4);

			_cefClient->getWindowManager()->SetIconWithNumber(browser, count);
		}
		else {
			res = CEF_ARGUMENT_ERROR;
		}
	}
	
	else if (method == L"SetTop") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_BOOL)){
			bool isTop = arguments->GetBool(4);
			_cefClient->getWindowManager()->SetTop(browser, isTop);
		}
		else {
			res = CEF_ARGUMENT_ERROR;
		}
	}
	else if (method == L"SetCaretPosition") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_INT) && (arguments->GetType(5) == VTYPE_INT)
			&& (arguments->GetType(6) == VTYPE_INT)){
			int x = arguments->GetInt(4);
			int y = arguments->GetInt(5);
			int height = arguments->GetInt(6);
			_cefClient->getWindowManager()->SetCaretPosition(browser, x, y, height);
		}
		else {
			res = CEF_ARGUMENT_ERROR;
		}
	}
	
	else if (method == L"GetDroppedFiles") {
		std::wstring names;
		res = _cefClient->getWindowManager()->GetDroppedFiles(browser, names);
		responseArgs->SetString(2, names);
	}
	else if (method == L"RegisterHotKey") {
		if ((argLen == 7) && (arguments->GetType(4) == VTYPE_INT) && (arguments->GetType(5) == VTYPE_INT) &&
			(arguments->GetType(6) == VTYPE_INT)){
			int id = arguments->GetInt(4);
			int modifiers = arguments->GetInt(5);
			int key = arguments->GetInt(6);
			_cefClient->getWindowManager()->RegisterHotKey(browser, id, modifiers, key);
		}
		else {
			res = CEF_ARGUMENT_ERROR;
		}
	}
	else if (method == L"UnRegisterHotKey") {
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_INT)){
			int id = arguments->GetInt(4);
			_cefClient->getWindowManager()->UnRegisterHotKey(browser, id);
		}
		else {
			res = CEF_ARGUMENT_ERROR;
		}
	}
	else if (method == L"GetWindowHwnd"){
		if ((argLen == 5) && (arguments->GetType(4) == VTYPE_STRING)){
			std::wstring name = arguments->GetString(4).ToWString();
			std::wstringstream stream;
			stream << (int64_t)_cefClient->getWindowManager()->GetWindowHwnd(name);
			responseArgs->SetString(2, stream.str());
		}
	}
	else {
		res = CEF_METHOD_NOTFOUND;
	}
}


void WindowExtension::ConverListToCaptions(CefRefPtr<CefListValue> captionList, std::vector<RECT> &captions)  {
		for (std::size_t i = 0; i < captionList->GetSize(); ++i) {
				if (captionList->GetType(i) == VTYPE_DICTIONARY) {
					CefRefPtr<CefDictionaryValue> caption = captionList->GetDictionary(i);

					RECT rect;
					if (caption->HasKey(L"top") && caption->GetType(L"top") == VTYPE_DOUBLE) {
						rect.top = (int)caption->GetDouble(L"top");
					}

					if (caption->HasKey(L"left") && caption->GetType(L"left") == VTYPE_DOUBLE) {
						rect.left = (int)caption->GetDouble(L"left");
					}

					if (caption->HasKey(L"width") && caption->GetType(L"width") == VTYPE_DOUBLE) {
						rect.right = (int)caption->GetDouble(L"width") +rect.left;
					}

					if (caption->HasKey(L"height") && caption->GetType(L"height") == VTYPE_DOUBLE) {
						rect.bottom = (int)caption->GetDouble(L"height") + rect.top;
					}

					captions.push_back(rect);
				}
		}
}

bool WindowExtension::ConvertDictionaryToWindowFeature(CefRefPtr<CefDictionaryValue> dictionary, 
	WindowFeature &feature) {
	if (dictionary->HasKey(L"isChild") && dictionary->GetType(L"isChild") == VTYPE_BOOL) {
		feature.isChild = dictionary->GetBool(L"isChild");
	}

	if (dictionary->HasKey(L"title") && dictionary->GetType(L"title") == VTYPE_STRING) {
		feature.title = dictionary->GetString(L"title").ToWString();
	}

	if (dictionary->HasKey(L"left") && dictionary->GetType(L"left") == VTYPE_DOUBLE) {
		feature.left = (int)dictionary->GetDouble(L"left");
	}

	if (dictionary->HasKey(L"top") && dictionary->GetType(L"top") == VTYPE_DOUBLE) {
		feature.top = (int)dictionary->GetDouble(L"top");
	}

	if (dictionary->HasKey(L"width") && dictionary->GetType(L"width") == VTYPE_DOUBLE) {
		feature.width = (int)dictionary->GetDouble(L"width");
	}

	if (dictionary->HasKey(L"height") && dictionary->GetType(L"height") == VTYPE_DOUBLE) {
		feature.height = (int)dictionary->GetDouble(L"height");
	}

	if (dictionary->HasKey(L"isMaximize") && dictionary->GetType(L"isMaximize") == VTYPE_BOOL) {
		feature.isMaximize = dictionary->GetBool(L"isMaximize");
	}

	if (dictionary->HasKey(L"centerScreen") && dictionary->GetType(L"centerScreen") == VTYPE_BOOL) {
		feature.centerScreen = dictionary->GetBool(L"centerScreen");
	}

	if (dictionary->HasKey(L"refreshIfExist") && dictionary->GetType(L"refreshIfExist") == VTYPE_BOOL) {
		feature.refreshIfExist = dictionary->GetBool(L"refreshIfExist");
	}

	if (dictionary->HasKey(L"modal") && dictionary->GetType(L"modal") == VTYPE_BOOL) {
		feature.modal = dictionary->GetBool(L"modal");
	}

	if (dictionary->HasKey(L"caption") && dictionary->GetType(L"caption") == VTYPE_LIST) {
		CefRefPtr<CefListValue> captions = dictionary->GetList(L"caption");
		ConverListToCaptions(captions, feature.captions);
	}

	if (dictionary->HasKey(L"resizable") && dictionary->GetType(L"resizable") == VTYPE_BOOL) {
		feature.resizable = dictionary->GetBool(L"resizable");
	}

	if (dictionary->HasKey(L"minWidth") && dictionary->GetType(L"minWidth") == VTYPE_DOUBLE) {
		feature.minWidth = (int)dictionary->GetDouble(L"minWidth");
	}

	if (dictionary->HasKey(L"minHeight") && dictionary->GetType(L"minHeight") == VTYPE_DOUBLE) {
		feature.minHeight = (int)dictionary->GetDouble(L"minHeight");
	}

	if (dictionary->HasKey(L"hasTaskBarIcon") && dictionary->GetType(L"hasTaskBarIcon") == VTYPE_BOOL) {
		feature.hasTaskBarIcon = dictionary->GetBool(L"hasTaskBarIcon");
	}

	if (dictionary->HasKey(L"isTop") && dictionary->GetType(L"isTop") == VTYPE_BOOL) {
		feature.isTop = dictionary->GetBool(L"isTop");
	}

	if (dictionary->HasKey(L"isShow") && dictionary->GetType(L"isShow") == VTYPE_BOOL) {
		feature.isShow = dictionary->GetBool(L"isShow");
	}

	if (dictionary->HasKey(L"behaviorIfSCClose") && dictionary->GetType(L"behaviorIfSCClose") == VTYPE_DOUBLE) {
		feature.behaviorIfSCClose = BehaviorIfSCClose((int)dictionary->GetDouble(L"behaviorIfSCClose"));
	}

	if (dictionary->HasKey(L"enableDrag") && dictionary->GetType(L"enableDrag") == VTYPE_BOOL) {
		feature.enableDrag = dictionary->GetBool(L"enableDrag");
	}

	if (dictionary->HasKey(L"isLayered") && dictionary->GetType(L"isLayered") == VTYPE_BOOL) {
		feature.isLayered = dictionary->GetBool(L"isLayered");
	}

	if (dictionary->HasKey(L"borderSpace")) {
		if (dictionary->GetType(L"borderSpace") == VTYPE_DOUBLE) {
			feature.leftBorder = feature.topBorder = feature.rightBorder = feature.bottomBorder = (int)dictionary->GetDouble(L"borderSpace");
		}
		else if (dictionary->GetType(L"borderSpace") == VTYPE_DICTIONARY){
			CefRefPtr<CefDictionaryValue> border = dictionary->GetDictionary(L"borderSpace");
			if (border->HasKey(L"leftBorder") && border->GetType(L"leftBorder") == VTYPE_DOUBLE) {
				feature.leftBorder = (int)border->GetDouble(L"leftBorder");
			}
			if (border->HasKey(L"topBorder") && border->GetType(L"topBorder") == VTYPE_DOUBLE) {
				feature.topBorder = (int)border->GetDouble(L"topBorder");
			}
			if (border->HasKey(L"rightBorder") && border->GetType(L"rightBorder") == VTYPE_DOUBLE) {
				feature.rightBorder = (int)border->GetDouble(L"rightBorder");
			}
			if (border->HasKey(L"bottomBorder") && border->GetType(L"bottomBorder") == VTYPE_DOUBLE) {
				feature.bottomBorder = (int)border->GetDouble(L"bottomBorder");
			}
		}
	}

	

	return true;
}

bool WindowExtension::ConvertDictionaryToMenuFeature(CefRefPtr<CefDictionaryValue> dictionary, 
	MenuFeature &feature) {
	if (dictionary->HasKey(L"id") && dictionary->GetType(L"id") == VTYPE_DOUBLE) {
		feature.id = (int)dictionary->GetDouble(L"id");
	}

	if (dictionary->HasKey(L"label") && dictionary->GetType(L"label") == VTYPE_STRING) {
		feature.label = dictionary->GetString(L"label");
	}

	if (dictionary->HasKey(L"icon") && dictionary->GetType(L"icon") == VTYPE_STRING) {
		feature.icon = dictionary->GetString(L"icon");
	}

	return true;
}

}