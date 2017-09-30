#ifndef PASSWORDRULE_H_
#define PASSWORDRULE_H_

#include "UCMessage.h"

namespace uc {

	class PasswordRule
	{
	public:
		PasswordRule() :_exprised_days(-1), _memory(-1), _complex(-1), _is_allow_change(1){};
		~PasswordRule() = default;

	public:
		void SetExprisedDays(int32_t days)
		{
			_exprised_days = days;
		}
		const int32_t & GetExprisedDays()
		{
			return _exprised_days;
		}

		void SetMemory(int32_t memory)
		{
			_memory = memory;
		}

		const int32_t &GetMemory()
		{
			return _memory;
		}

		void SetComplex(int32_t complex)
		{
			_complex = complex;
		}

		const int32_t &GetComplex()
		{
			return _complex;
		}

		void SetAllowChange(int32_t allow_change)
		{
			_is_allow_change = allow_change;
		}

		const int32_t &GetAllowChange()
		{
			return _is_allow_change;
		}
	private:
//		"pwd_exprised_days" : 1 // 1-30d 2-60d 3-90d 4-180d 5-永不过期，
//		"pwd_memory" : 1 // 1-3次 2-5次 3-10次 4-不记忆
//		"pwd_complex" : 1 //1- 8到20位不限类型 2-8到20位数字字母组合 3- 8到20位数字,符号与字母组合
		int32_t _exprised_days;
		int32_t _memory;
		int32_t _complex;

		int32_t _is_allow_change; // 默认是1，就是说默认允许修改密码

	};

} /* namespace uc */

#endif /* PASSWORDRULE_H_ */
