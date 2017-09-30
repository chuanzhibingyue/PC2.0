//
//  TangUser.h
//  BeeCoference
//
//  Created by Hualong Zhang on 15/5/20.
//  Copyright (c) 2015年 Hualong Zhang. All rights reserved.
//

#ifndef __BeeCoference__TangUser__
#define __BeeCoference__TangUser__

#include "ITangUser.h"

namespace uc {

class TangUser : public ITangUser {
public:
	TangUser();
	~TangUser();
	virtual unsigned int ReturnUserID() const; //tang user id
	virtual const std::string &ReturnUserName() const; //bee user id
	virtual bool IsMute() const;
	virtual AudioType ReturnAudioType() const;
	virtual const std::string &ReturnPhoneNumber() const;
	virtual const std::string &ReturnDescription() const;
	void SetDescription();

    unsigned int tangUserID;
    std::string userName;
    bool isMute;
    AudioType audioType;
    std::string phoneNumber;
	std::string description;
};

}
#endif /* defined(__BeeCoference__TangUser__) */
