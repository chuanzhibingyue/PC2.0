

#include <stdio.h>
#include "TangUser.h"

namespace uc {
	TangUser::TangUser()
	:tangUserID(0)
	,userName("")
	,isMute(false)
	,audioType(NONE)
	,phoneNumber("")
	,description(""){

	}

	TangUser::~TangUser() {

	}

	void TangUser::SetDescription() {
		char descriptionTmp[260] = {0};
		sprintf(descriptionTmp, "tangUserID: %d, userName: %s, audioType:%d, isMute: %d, phoneNumber: %s",
			tangUserID, userName.c_str(), audioType, isMute, phoneNumber.c_str());
		description = descriptionTmp;
	}

	unsigned int TangUser::ReturnUserID() const{
		return tangUserID;
	}

	const std::string &TangUser::ReturnUserName() const{
		return userName;
	}

	bool TangUser::IsMute() const{
		return isMute;
	}
	
	AudioType TangUser::ReturnAudioType() const{
		return audioType;
	}

	const std::string &TangUser::ReturnPhoneNumber() const{
		return phoneNumber;
	}
	
	const std::string &TangUser::ReturnDescription() const{
		return description;
	}
}
