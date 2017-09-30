#pragma once

#include "IP2PService.h"

#include "Includes\IOP2PMgr.h"

namespace uc {
class p2p_service : public IP2PService, public IP2PSink
{ // 处理通过文件消息交互窗口
public:
	p2p_service(IUCEngine *engine);
	~p2p_service(void);

	virtual int32_t Initialize();
	virtual int32_t UploadFile();
	virtual int32_t DownloadFile();

protected:
	virtual void onNotify(int nType) const;
	virtual void onUploadFile();
	virtual void onDownloadFile();

protected:
	int32_t getLocalID();

protected:
	IP2PMgr *m_lpP2PMgr;
};
}

