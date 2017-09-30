#ifndef CREATE_ENGINE_CONFIG_H_
#define CREATE_ENGINE_CONFIG_H_
#include <string>
using namespace std;

void CreateEngineConfig(UCEngineConfig &ucConfig);

void UC_EXPECT_EQ(boost::recursive_mutex &mutex, boost::condition_variable_any &condition, int32_t expected, int32_t &actual);
void UC_Lock_Notify(boost::recursive_mutex &mutex, boost::condition_variable_any &condition);
#endif