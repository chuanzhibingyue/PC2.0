
#include <string>
#include "gtest/gtest.h"
#include "network/NetworkManager.h"

using namespace std;

class NetworkManagerTest : public ::testing::Test {
public:
	NetworkManagerTest() {};
	virtual ~NetworkManagerTest() {};
	
protected:
	virtual void SetUp() {};
	virtual void TearDown() {};

};

TEST(NetworkManagerTest, StringConvert) {
	wstring value = L"test";
	string utf8 = "123";
/*
	utf8 = uc::NetworkManager::ToUTF8Str(value);
	EXPECT_STREQ("test", utf8.c_str());
	EXPECT_STREQ("123", utf8.c_str());*/
}
