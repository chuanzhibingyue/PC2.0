#pragma once

#include "UCSDK.h"
#include "IUCEngine.h"
#include "HTTPRequestEvent.h"

// ui ½»»¥½Ó¿Ú
namespace uc {
	interface UC_API IP2PServiceEvent;
	interface UC_API IP2PService : public IUCService, HTTPRequestEvent, Event<IP2PServiceEvent> {
		virtual ~IP2PService(){};

		virtual int32_t Initialize() = 0;
		
		virtual int32_t UploadFile() = 0;

		virtual int32_t DownloadFile() = 0;
	};

	interface UC_API IP2PServiceEvent {
		virtual ~IP2PServiceEvent(){};

		virtual void OnUploadFile() = 0;

		virtual void OnDownloadFile() = 0;
	};
}