#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "common/platform.h"

namespace ucclient {

class Environment {
public:
	static int32_t EnableAutoStart();

	static int32_t IsAutoStart();

	static int32_t DisableAutoStart();
};

}
#endif