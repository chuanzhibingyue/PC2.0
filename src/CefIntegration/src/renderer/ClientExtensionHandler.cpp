#include "renderer/ClientExtensionHandler.h"
#include "CefJSConversion.h"
#include "ClientDefs.h"
#include "utils/FileUtil.h"
#include "utils/ImageUtil.h"
#include "utils/Font.h"
#include "LogManager.h"
#include <cassert>
#include <codecvt>

namespace cefIntegration {

using namespace uc;

ClientExtensionHandler::ClientExtensionHandler() : _requestId(0) {
}

bool ClientExtensionHandler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, 
	const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, 
	CefString& exception) {
    if (name == L"CallNativeMethodAync") {
        CallNativeMethodAync(arguments, L"CallNativeMethodAync");
    }
    else if (name == L"JSCommond") {
    	CallNativeMethodAync(arguments, L"JSCommond");
    }
	else if (name == L"GetFileSize"){
		if (arguments.size() == 1 && arguments[0]->IsString()) {
			retval = CefV8Value::CreateDouble(FileUtil::GetFileSize(arguments[0]->GetStringValue().ToWString()));
		}
		else {
			retval = CefV8Value::CreateDouble(0);
		}
	}
	else if (name == L"FileExist"){
		if (arguments.size() == 1 && arguments[0]->IsString()) {
			retval = CefV8Value::CreateBool(FileUtil::FileExist(arguments[0]->GetStringValue().ToWString()));
		}
		else {
			retval = CefV8Value::CreateBool(false);
		}
	}
	else if (name == L"OpenFile") {
		if (arguments.size() == 1 && arguments[0]->IsString()) {
			retval = CefV8Value::CreateInt(FileUtil::OpenFile(arguments[0]->GetStringValue().ToWString()));
		}
		else {
			retval = CefV8Value::CreateInt(CEF_ARGUMENT_ERROR);
		}
	}
	else if (name == L"DeleteFile") {
		if (arguments.size() == 1 && arguments[0]->IsString()) {
			retval = CefV8Value::CreateInt(FileUtil::DeleteFileEx(arguments[0]->GetStringValue().ToWString()));
		}
		else {
			retval = CefV8Value::CreateInt(CEF_ARGUMENT_ERROR);
		}
	}
	else if (name == L"OpenFolder") {
		if (arguments.size() == 1 && arguments[0]->IsString()) {
			retval = CefV8Value::CreateInt(FileUtil::OpenFolder(arguments[0]->GetStringValue().ToWString()));
		}
		else {
			retval = CefV8Value::CreateInt(CEF_ARGUMENT_ERROR);
		}
	}
	else if (name == L"CopyFile") {
		if (arguments.size() == 2 && arguments[0]->IsString() && arguments[1]->IsString()) {
			retval = CefV8Value::CreateInt(FileUtil::CopyFileEX(arguments[0]->GetStringValue().ToWString(), arguments[1]->GetStringValue().ToWString()));
		}
		else {
			retval = CefV8Value::CreateInt(CEF_ARGUMENT_ERROR);
		}
	}
	else if (name == L"SaveFile") {
		if (arguments.size() == 2 && arguments[0]->IsString() && arguments[1]->IsString()) {
			std::wstring content = arguments[1]->GetStringValue().ToWString();
			
			// util库未从sdk中独立，故先拷贝过来
			std::string utf8;
			std::wstring_convert<std::codecvt_utf8<wchar_t> > convert;
			try {
				utf8 = convert.to_bytes(content);
			} catch(const std::range_error& e) {
				utf8 = convert.to_bytes(content.substr(0, convert.converted()));
			}

			retval = CefV8Value::CreateInt(FileUtil::SaveFile (arguments[0]->GetStringValue().ToWString(), utf8));
		}
		else {
			retval = CefV8Value::CreateInt(CEF_ARGUMENT_ERROR);
		}
	}
	else if (name == L"GetDirectoryContent") {
		if (arguments.size() == 1 && arguments[0]->IsString()) {
			std::vector<std::wstring> files;
			FileUtil::GetDirectoryContent(arguments[0]->GetStringValue().ToWString(), files);

			retval = CefV8Value::CreateArray(files.size());
			for (int i = 0; i < files.size(); ++i) {
				retval->SetValue(i, CefV8Value::CreateString(files[i]));
			}
		}
		else {
			retval = CefV8Value::CreateArray(0);
		}
	}
	else if (name == L"CropImage") {
		if (arguments.size() == 8 && arguments[0]->IsString() && arguments[1]->IsInt() && arguments[2]->IsInt() && 
			arguments[3]->IsInt() && arguments[4]->IsInt() && arguments[5]->IsInt() && arguments[6]->IsInt() && arguments[7]->IsString()) {
			std::wstring imagePath = arguments[0]->GetStringValue().ToWString();
			int x = arguments[1]->GetIntValue();
			int y = arguments[2]->GetIntValue();
			int width = arguments[3]->GetIntValue();
			int height = arguments[4]->GetIntValue();
			int targetWidth = arguments[5]->GetIntValue();
			int targetHeight = arguments[6]->GetIntValue();
			std::wstring savePath = arguments[7]->GetStringValue().ToWString();

			retval = CefV8Value::CreateInt(ImageUtil::Crop(imagePath, x, y, width, height, targetWidth, targetHeight, savePath));
		}
		else {
			retval = CefV8Value::CreateInt(CEF_ARGUMENT_ERROR);
		}
	}
	else if (name == L"IsCapsLockPressed") {
		bool isPressed = ((::GetKeyState(VK_CAPITAL) && 0x8000) == 1) ? true : false;
		retval = CefV8Value::CreateBool(isPressed);
	}
	else if (name == L"GetSystemFonts") {	
		std::vector<std::wstring> fonts;
		Font::EnumerateFonts(fonts);

		retval = CefV8Value::CreateArray(fonts.size());

		for (std::size_t i = 0; i < fonts.size(); ++i) {
			retval->SetValue(i, CefV8Value::CreateString(fonts[i]));
		}
	}
    else {
    	return false;
    }
    return true;
}

bool ClientExtensionHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, 
	CefProcessId source_process, CefRefPtr<CefProcessMessage> message) {
	if (message->GetName() == "InvokeCallback") {
		CefRefPtr<CefListValue> msgArgs = message->GetArgumentList();
		int requestId = msgArgs->GetInt(0);
        int argLen = msgArgs->GetSize();
		
		JSCallbackMap::iterator iter = _callbackMap.find(requestId);
		if (iter != _callbackMap.end()) {
			CefRefPtr<CefV8Context> context = iter->second.first;
		    CefRefPtr<CefV8Value> func = iter->second.second;
		    CefV8ValueList arguments;

		    context->Enter();
            
		    if (context->GetBrowser()) {
		    	CefRefPtr<CefV8Value> args = CefV8Value::CreateArray(argLen - 1);

		        for (int i = 1; i < argLen; i++) {
					CefRefPtr<CefV8Value> value;
					CefJSConversion::ListToV8Value(msgArgs, i, value);
		        	args->SetValue(i - 1, value);
		        }
                
		        arguments.push_back(args);
		        func->ExecuteFunction(NULL, arguments);
			}
            
		    context->Exit();
            
		    _callbackMap.erase(iter);
		}
		
		return true;
	}
	
	return false;	
}
		
void ClientExtensionHandler::OnContextReleased(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) {
	// 浏览器的context释放，去除该context引用
	if (!_callbackMap.empty()) {
		for (JSCallbackMap::iterator iter = _callbackMap.begin(); 
			iter != _callbackMap.end();) {
				if (iter->second.first->IsSame(context)) {
				iter = _callbackMap.erase(iter);
			}
			else {
				++iter;
			}
		}
	}
}

void ClientExtensionHandler::CallNativeMethodAync(const CefV8ValueList& arguments, const CefString& messageName) {
    CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
    assert(browser.get());
	
    CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(messageName);
    CefRefPtr<CefListValue> msgArgs = message->GetArgumentList();
    CefJSConversion::V8ValueToList(CefV8Value::CreateInt(_requestId), 0, msgArgs);
	
	bool hasCallback = false;
	int arglen = arguments.size();
	int realArglen = arguments.size();
	if ((arglen > 1) && (arguments[arglen-1]->IsFunction())) {
		//最后一个参数是函数，作为callback
		_callbackMap[_requestId] = std::make_pair(CefV8Context::GetCurrentContext(), 
			arguments[arglen - 1]);
		hasCallback = true;
		realArglen--;
	}
	CefJSConversion::V8ValueToList(CefV8Value::CreateBool(hasCallback), 1, msgArgs);
	
    // 不打印日志打印的方法
    if (arglen >= 2 && 0 !=arguments[1]->GetStringValue().ToString().compare("WriteLog")) {
		UC_LOG(INFO) << "ClientExtensionHandler::CallNativeMethodAync, requestId: " << _requestId << ",  arglen: " << arglen
			<< ", hasCallback: " << hasCallback << ", service: " << arguments[0]->GetStringValue().ToString() <<
			", method: " << arguments[1]->GetStringValue().ToString();
	}

	bool res =  true;
	int i = 0;
	for (; i < realArglen && res; i++) {
        res = CefJSConversion::V8ValueToList(arguments[i], i + 2, msgArgs);
    }

	if (!res) {
		UC_LOG(ERROR) << "invalid parameters from JS, index: " << --i;

		if (hasCallback){
			_callbackMap.erase(_requestId);

			CefRefPtr<CefV8Value> args = CefV8Value::CreateArray(1);
			args->SetValue(0, CefV8Value::CreateInt(CEF_PARAMETER_ERROR));
			CefV8ValueList v8List;
			v8List.push_back(args);

			CefV8Context::GetCurrentContext()->Enter();
			arguments[arglen - 1]->ExecuteFunction(NULL, v8List);
			CefV8Context::GetCurrentContext()->Exit();
		}
		
		return;
	}

    browser->SendProcessMessage(PID_BROWSER, message);
	
	_requestId++;
}

}