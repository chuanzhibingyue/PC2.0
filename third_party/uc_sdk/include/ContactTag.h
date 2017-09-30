/*
 * ContactTag.h
 *
 *  Created on: May 30, 2015
 *      Author: gang wang
 */

#ifndef CONTACT_TAG_H_
#define CONTACT_TAG_H_

#include <string>

#include "UCSDK.h"

namespace uc {

class UC_API ContactTag {
public:
    ContactTag();
    ContactTag (const ContactTag& tag);

    ContactTag& operator=(const ContactTag& other);

    enum Scope{
		CTS_DEFAULT = 0,            // 所有员工可填写
		CTS_ADMIN_ONLY,             // 管理员填写
		CTS_STAFF_ONLY              // 员工填写
	};

	enum InputType{
		CTIT_SINGLE_LINE_TEXT = 1,  // 单行文本
		CTIT_MULTI_LINE_TEXT,       // 多行文本
		CTIT_NUMBER,                // 数字或电话号码
		CTIT_DATE                   // 日期
	};

	enum Type{
		CTT_BASIC = 1,              // 基本标签
		CTT_CUSTOM,                 // 自定义标签
	};

	/**
	 * 更新除_id和_userId以外的属性
	 */
	void Update(const ContactTag& tag);

	int32_t GetId() const {
		return _id;
	}

	void SetId(int32_t id) {
		_id = id;
	}

	InputType GetInputType() const {
		return _inputType;
	}

	void SetInputType(InputType inputType) {
		_inputType = inputType;
	}

	const std::string& GetName() const {
		return _name;
	}

	void SetName(const std::string& name) {
		_name = name;
	}

	Scope GetScope() const {
		return _scope;
	}

	void SetScope(Scope scope) {
		_scope = scope;
	}

	int32_t GetState() const {
		return _state;
	}

	void SetState(int32_t state) {
		_state = state;
	}

	Type GetType() const {
		return _type;
	}

	void SetType(Type type) {
		_type = type;
	}

	int32_t GetUserId() const {
		return _userId;
	}

	void SetUserId(int32_t userId) {
		_userId = userId;
	}

	const std::string& GetValue() const {
		return _value;
	}

	void SetValue(const std::string& value) {
		_value = value;
	}

private:
    int32_t _id;
    std::string _name;
    std::string _value;
    InputType _inputType;
    int32_t _userId;
    Scope _scope;
    Type _type;
    int32_t _state;                 // 标签开启状态（0：禁用，1启用）
};

}
#endif
