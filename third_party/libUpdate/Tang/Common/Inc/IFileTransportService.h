#ifndef IFILETRANSPORTSERVICE_H
#define IFILETRANSPORTSERVICE_H

#include "IMessageProvider.h"
#include "IConfiguration.h"
#include "IExternal.h"
#include "IService.h"

class IFileTransportService : public IService
{
public:
    virtual HRESULT Init(IMessageProvider* pMessageDispatcher,
        IConfiguration* pConfiguration, IExternal* pExternal, IDispatch* pIEInstance) = 0;

    virtual HRESULT UnInit() = 0;
};

#endif // IFILETRANSPORTSERVICE_H
