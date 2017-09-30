/*
 * UCClientRenderDelegate.h
 *
 *  Created on: August 4, 2015
 *      Author: gang wang
 */

#ifndef UCCLIENT_RENDER_DELEGATE_H_
#define UCCLIENT_RENDER_DELEGATE_H_
#include "Delegate.h"

namespace cefIntegration{
class CefClient;
}

namespace ucSubProcess {

class UCClientRenderDelegate : public cefIntegration::RenderDelegate {
public:
	UCClientRenderDelegate(cefIntegration::CefClient * cefClient);

	//virtual void GetExtensionCode(std::string& code);

	void OnWebKitInitialized();
private:
	cefIntegration::CefClient *_cefClient;

	IMPLEMENT_REFCOUNTING(UCClientRenderDelegate);
};
	
}

#endif