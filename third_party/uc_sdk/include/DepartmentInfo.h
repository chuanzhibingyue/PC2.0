/*
 * DepartmentInfo.h
 *
 *  Created on: May 7, 2015
 *      Author: gang wang
 */

#ifndef DEPARTMENT_INFO_H_
#define DEPARTMENT_INFO_H_

#include <string>
#include <vector>

#include "UCSDK.h"
#include "ContactInfo.h"

namespace uc {

struct UC_API DepartmentInfo {

	DepartmentInfo () : _deptId(-1), _deptName(""), _parentId(0),
		_staffNum(0), _totalNum(0), _onlineNum(0){
	}

	~DepartmentInfo () {
		for (std::vector<ContactInfo*>::iterator iter = _contacts.begin(); 
			iter != _contacts.end(); ++iter) {
			delete *iter;
		}

		_contacts.clear();

		for (std::vector<DepartmentInfo*>::iterator iter = _subDepartments.begin(); 
			iter != _subDepartments.end(); ++iter) {
			delete *iter;
		}

		_subDepartments.clear();
	}

	/**
     * 获取部门id
     * @return 部门id
     */
	int GetId() const {
		return _deptId;
	}

	/**
     * 设置部门id
     * @param deptId 部门id
     */
	void SetId(int deptId) {
		_deptId = deptId;
	}

	/**
     * 获取部门名称
     * @return 姓名
     */
	const std::string& GetDepartName() const {
		return _deptName;
	}

	/**
     * 设置部门名称
     * @param deptId 部门名称
     */
	void SetDepartName(const std::string &deptName) {
		_deptName = deptName;
	}

	/**
     * 获取父级部门ID
     * @return 父级部门ID
     */
	int GetParentId() const {
		return _parentId;
	}

	/**
     * 设置父级部门ID
     * @param parentId 父级部门ID
     */
	void SetParentId(int parentId) {
		_parentId = parentId;
	}

	/**
     * 获取直属员工数量
     * @return 直属员工数量
     */
	int GetStaffNum() const {
		return _staffNum;
	}

	/**
     * 设置直属员工数量
     * @param staffNum 直属员工数量
     */
	void SetStaffNum(int staffNum) {
		_staffNum = staffNum;
	}

	/**
     * 获取所有员工数量
     * @return 所有员工数量
     */
	int GetTotalNum() const {
		return _totalNum;
	}

	/**
     * 设置所有员工数量
     * @param totalNum 所有员工数量
     */
	void SetTotalNum(int totalNum) {
		_totalNum = totalNum;
	}

	/**
     * 获取在线人数
     * @return 在线人数
     */
	int GetOnlineNum() const {
		return _onlineNum;
	}

	/**
     * 设置在线人数
     * @param onlineNum 在线人数
     */
	void SetOnlineNum(int onlineNum) {
		_onlineNum = onlineNum;
	}

	/**
     * 获取员工信息
     * @return 员工信息
     */
	const std::vector<ContactInfo*>& GetStaffs() const {
		return _contacts;
	}
	
	/**
     * 设置员工信息
     * @param onlineNum 员工信息
     */
	void SetStaffs(const std::vector<ContactInfo*>& contacts) {
		_contacts = contacts;
	}

	/**
     * 获取子部门
     * @return 子部门
     */
	const std::vector<DepartmentInfo*>& GetSubDepart() const {
		return _subDepartments;
	}
	
	/**
     * 设置子部门
     * @param onlineNum 子部门
     */
	void SetSubDepart(const std::vector<DepartmentInfo*>& departs) {
		_subDepartments = departs;
	}
private:
	// 部门ID
	int _deptId;

	// 部门名称
	std::string _deptName;

	// 父级部门ID（0表示没有父级部门）
	int _parentId;

	// 直属员工数量
	int _staffNum;

	// 所有员工数量
	int _totalNum;
	
	// 在线人数
	int _onlineNum;

	// 员工信息
	std::vector<ContactInfo*> _contacts;
	
	std::vector<DepartmentInfo*> _subDepartments;
};

}

#endif
