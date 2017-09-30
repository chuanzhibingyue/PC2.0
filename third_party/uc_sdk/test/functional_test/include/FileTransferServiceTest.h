#ifndef FILETRANSFER_SERVICE_TEST_H_
#define FILETRANSFER_SERVICE_TEST_H_
#include <string>
#include "gtest/gtest.h"
#include "IUCEngine.h"
#include "ClientInfo.h"

using namespace uc;

class FileTransferServiceTest : public ::testing::Test {
public:
	FileTransferServiceTest();
	virtual ~FileTransferServiceTest() {}
public:
	static void SetUpTestCase();
	static void TearDownTestCase();
	void SetUp();
	void TearDown();	
public:
	IUCEngine *_ucEngine;
	ClientInfo _info;
};

#endif