/*
 * FunctionalTest.cpp
 *
 *  Created on: 2015年6月24日
 *      Author: jianxue
 */

#include "ClientServiceTest.h"
#include <windows.h>
#include "gtest/gtest.h"
#include "utils/callback/ExecuteCallback.h"

class UCEnvironment : public testing::Environment {
public:
	UCEnvironment() {}
	virtual ~UCEnvironment() {}
public:
	virtual void SetUp() {
		std::cout << "uc function test start......." << std::endl;
	}

	virtual void TearDown() {
		std::cout << "uc function test end......." << std::endl;
	}
};

struct MainArgs{
	int argc; 
	char** argv;
};

DWORD WINAPI GTestSubThreadProc(PVOID pvParam){
  MainArgs args = *((MainArgs*)pvParam);
  
  testing::AddGlobalTestEnvironment(new UCEnvironment); 
  testing::GTEST_FLAG(filter) = "AudioServiceTest*";
  testing::InitGoogleTest(&args.argc, args.argv);
  RUN_ALL_TESTS();
  ::PostMessageA(NULL, WM_QUIT, 0, 0);
  return 0;
}

int main(int argc, char** argv) {
	AttachMainThreadMessageProc(new DefaultMessageProc());

	MainArgs args = {argc, argv};
	DWORD dwThreadId;
	HANDLE hThread = CreateThread(NULL, 0, GTestSubThreadProc, (PVOID)&args, 0, &dwThreadId);
	CloseHandle(hThread);

	MSG msg;


	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_QUIT) {
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DetachMainThreadMessageProc();

	return 0;
}



