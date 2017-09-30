LOCAL_PATH := $(call my-dir)

#
#sdk
#

include $(CLEAR_VARS)
LOCAL_MODULE := sdk
LOCAL_MODULE_FILENAME := libsdk

LOCAL_SRC_FILES := \
../../../src/components/db/src/AccountDao.cpp \
../../../src/components/db/src/AppDao.cpp \
../../../src/components/db/src/ConferenceDao.cpp \
../../../src/components/db/src/ConfigDao.cpp \
../../../src/components/db/src/ContactDao.cpp \
../../../src/components/db/src/ConversationDao.cpp \
../../../src/components/db/src/CppSQLite3.cpp \
../../../src/components/db/src/DBManager.cpp \
../../../src/components/db/src/DBUpgrade.cpp \
../../../src/components/db/src/FileTransferDao.cpp \
../../../src/components/db/src/GroupDao.cpp \
../../../src/components/db/src/MessageDao.cpp \
../../../src/components/db/src/ProxyInfoDao.cpp \
../../../src/components/db/src/SQLiteManager.cpp \
../../../src/components/network/HTTPHandler.cpp \
../../../src/components/network/HTTPProxy.cpp \
../../../src/components/network/HTTPRequest.cpp \
../../../src/components/network/HTTPRequestEvent.cpp \
../../../src/components/network/HTTPResponse.cpp \
../../../src/components/network/NetworkManager.cpp \
../../../src/components/network/UCASConnection.cpp \
../../../src/components/parser/msg_parser/src/AppMessageType.cpp \
../../../src/components/parser/msg_parser/src/MessageParser.cpp \
../../../src/components/parser/msg_parser/src/MessageStream.cpp \
../../../src/components/parser/msg_parser/src/UCMessage.cpp \
../../../src/components/parser/msg_parser/src/UCMessageType.cpp \
../../../src/components/parser/msg_parser/src/UCTypes.cpp \
../../../src/services/calendar_service/CalendarMsgProcessor.cpp \
../../../src/services/calendar_service/CalendarRequestFactory.cpp \
../../../src/services/calendar_service/CalendarService.cpp \
../../../src/services/calendar_service/ICalendarParser/date.cpp \
../../../src/services/calendar_service/ICalendarParser/element.cpp \
../../../src/services/calendar_service/ICalendarParser/icalendar.cpp \
../../../src/services/client_service/ClientService.cpp \
../../../src/services/client_service/LoginInfo.cpp \
../../../src/services/client_service/ServerInfo.cpp \
../../../src/services/contact_service/ContactEventDispatcher.cpp \
../../../src/services/contact_service/ContactInfo.cpp \
../../../src/services/contact_service/ContactInfoImpl.cpp \
../../../src/services/contact_service/ContactMsgProcessor.cpp \
../../../src/services/contact_service/ContactRequestFactory.cpp \
../../../src/services/contact_service/ContactResponseParser.cpp \
../../../src/services/contact_service/ContactService.cpp \
../../../src/services/contact_service/ContactTag.cpp \
../../../src/services/contact_service/contact_cache/ContactDelayLoader.cpp \
../../../src/services/contact_service/contact_cache/RegularUpdatedCache.cpp \
../../../src/services/contact_service/DefaultContactEventDispatcher.cpp \
../../../src/services/filetransfer_service/FileTransferService.cpp \
../../../src/services/filetransfer_service/FSRequestAssist.cpp \
../../../src/services/filetransfer_service/FSResponseAssist.cpp \
../../../src/services/filetransfer_service/Task.cpp \
../../../src/services/filetransfer_service/UCFile.cpp \
../../../src/services/message_service/DocumentInfo.cpp \
../../../src/services/message_service/GroupInfo.cpp \
../../../src/services/message_service/GroupService.cpp \
../../../src/services/message_service/GroupServiceNotify.cpp \
../../../src/services/message_service/GroupServiceRequest.cpp \
../../../src/services/message_service/GroupServiceResponse.cpp \
../../../src/services/message_service/IMService.cpp \
../../../src/services/message_service/MessageInfo.cpp \
../../../src/services/message_service/MessageProcessor.cpp \
../../../src/services/microblog_service/MicroblogService.cpp \
../../../src/services/openapi_service/OpenApiService.cpp \
../../../src/services/search_service/SearchService.cpp \
../../../src/services/sharing_service/SharingService.cpp \
../../../src/services/store_service/ConversationStore.cpp \
../../../src/services/store_service/GroupListStore.cpp \
../../../src/services/store_service/MessageStore.cpp \
../../../src/services/store_service/StoreService.cpp \
../../../src/services/video_service/VideoService.cpp \
../../../src/UCEngine.cpp \
../../../src/utils/HTTPResponseAssist.cpp \
../../../src/utils/md5/md5.cpp \
../../../src/utils/TaskExecutor.cpp \
../../../src/utils/ThreadPool.cpp \
../../../src/utils/Timer.cpp

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../include \
$(LOCAL_PATH)/../../../include/base \
$(LOCAL_PATH)/../../../src \
$(LOCAL_PATH)/../../../src/common \
$(LOCAL_PATH)/../../../src/utils \
$(LOCAL_PATH)/../../../src/components \
$(LOCAL_PATH)/../../../src/components/network \
$(LOCAL_PATH)/../../../src/components/parser/msg_parser/include \
$(LOCAL_PATH)/../../../src/components/db/include \
$(LOCAL_PATH)/../../../src/services \
$(LOCAL_PATH)/../../../third_party/log_manager \
$(LOCAL_PATH)/../../../third_party/curl/include \
$(LOCAL_PATH)/../../../third_party/gtest/include \
$(LOCAL_PATH)/../../../third_party \
$(LOCAL_PATH)/../../../third_party/thrift/include \
$(LOCAL_PATH)/../../../third_party/sqlite3/include \
$(LOCAL_PATH)/../../../third_party/vld/include \
$(LOCAL_PATH)/../../../third_party/boost/include \
$(LOCAL_PATH)/../../../third_party/tang/include \
$(LOCAL_PATH)/../../../third_party/libproxy/include \
$(LOCAL_PATH)/../../../third_party/common/include \
$(LOCAL_PATH)/../../../third_party/common/include/common \
$(LOCAL_PATH)/../../../third_party/zlib \
$(LOCAL_PATH)/../../../third_party/rapidjson/include \
$(NDK_ROOT)/platforms/android-9/arch-arm/usr/include/android

LOCAL_CPPFLAGS += -fexceptions -frtti -std=c++11 -pthread -fexceptions

NDK_TOOLCHAIN_VERSION :=4.8

LOCAL_LDLIBS := -llog \
-landroid \
-L$(LOCAL_PATH)/../../../third_party/zlib/lib/android/$(TARGET_ARCH_ABI)/ -lz \
-L$(LOCAL_PATH)/../../../third_party/common/lib/android/$(TARGET_ARCH_ABI)/ -lcommon \
-L$(LOCAL_PATH)/../../../third_party/sqlite3/lib/android/$(TARGET_ARCH_ABI)/ -lsqlite3 \
-L$(LOCAL_PATH)/../../../third_party/google-glog/lib/android/$(TARGET_ARCH_ABI)/ -lglog \
-L$(LOCAL_PATH)/../../../third_party/log_manager/lib/android/$(TARGET_ARCH_ABI)/ -llogmanager \
-L$(LOCAL_PATH)/../../../third_party/google-glog/lib/android/$(TARGET_ARCH_ABI)/ -lglog \
-L$(LOCAL_PATH)/../../../third_party/thrift/lib/android/$(TARGET_ARCH_ABI)/ -lthrift \
-L$(LOCAL_PATH)/../../../third_party/openssl/lib/android/$(TARGET_ARCH_ABI)/ -lssl -lcrypto \
-L$(LOCAL_PATH)/../../../third_party/curl/lib/android/$(TARGET_ARCH_ABI)/ -lcurl \
-L$(LOCAL_PATH)/../../../third_party/boost/lib/android/$(TARGET_ARCH_ABI)/ -lboost

include $(BUILD_SHARED_LIBRARY)
