//
//  TangUser.h
//  BeeCoference
//
//  Created by Hualong Zhang on 15/5/20.
//  Copyright (c) 2015年 Hualong Zhang. All rights reserved.
//

#ifndef __BeeCoference__ITangUser__
#define __BeeCoference__ITangUser__

#include <string>

namespace uc {

typedef enum {
    VOIP = 1,
    PHONE,
    NONE
}AudioType;

#define PROPERTY_AUDIOTYPE          1
#define PROPERTY_MUTE               2
#define PROPERTY_AUDIOTYPE_MUTE     3

class ITangUser
{
public:
	virtual unsigned int ReturnUserID() const = 0;
	virtual const std::string &ReturnUserName() const = 0;
	virtual bool IsMute() const = 0;
	virtual AudioType ReturnAudioType() const = 0;
	virtual const std::string &ReturnPhoneNumber() const = 0;
	virtual const std::string &ReturnDescription() const = 0;
};

}
#endif /* defined(__BeeCoference__ITangUser__) */
