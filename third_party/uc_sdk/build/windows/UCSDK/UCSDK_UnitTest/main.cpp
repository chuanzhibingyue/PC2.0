
#include <iostream>
#include "gtest/gtest.h"
#include "IUCEngine.h"
#include "IClientService.h"

using namespace std;
using namespace uc;

/*
class UCMockClient : public IClientServiceEvent {
public:
	UCMockClient() {

	}

	virtual ~UCMockClient() {

	}

public:
	virtual void OnLogin(int32_t Result) {
		wcout << L"OnLogin event gets called: " << Result << endl << endl;
	}

	virtual void OnLogout(int32_t Result) {
		wcout << L"OnLogout:" << Result << endl;
	}
};
*/

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);

	 RUN_ALL_TESTS();


	 while(getmessage\) {
	 }
	/*
	UCMockClient MockClient;
	IUCEngine *TestEngine = CreateUCEngine();
	IClientService *ClientService = NULL;

	wstring Account = L"jian.xue@quanshi.com";
	wstring Password = L"11111111";
	wstring ClientInfo = L"{\"client_id\":\"3f5edcb567594ff83c1af5693740d4e71e0a222949705aaeba5dc613462977ba\",\"lang\":\"zh-Hans\",\"pin_number\":\"E40D42D2-4C88-47D0-8029-03DEEC649312\",\"version\":\"0.9.580.16577\"}";
	uint16_t ClientType = UCCLIENT_IOS;

	TestEngine->HelloEngine();
	ClientService = (IClientService*)TestEngine->QueryService(UCClientService);

	ClientService->registerListener(&MockClient);
	ClientService->Login(Account, Password, ClientInfo, ClientType);
	*/
	return 0;

}