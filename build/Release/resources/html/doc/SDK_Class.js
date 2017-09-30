/**
 * Created by waouyang on 15/8/11.
 * This file define object defined in SDK
 */
ContactInfo = {
    userId: 0,
    siteId : 0,
    cardVersion: 0,
    status: 0, //1： online 2: busy 3:away 4: in meeting 5: offline
    displayName: '',
    pinyin: '',
    sex: 0, //1: 男 2: 女
    account: '',
    email: '',
    mobile: '',
    workPhone: '',
    localAvatar: '',
    remoteAvatarUrl: '',
    personalSign: '',
    departmentId: 0,
    departmentName : '',
    position: '',
    superiorId: 0,
    superiorName: '',
    colleagueNum: 0,
    type: 0, //0: 常用联系人 1: 登录用户 2：其他联系人
    tags: []
};
TagInfo = {
    id: 0,
    inputType: 0, //输入类型 1： 单行文本 2： 多行文本 3： 数字或电话 4： 日期
    name: '',
    value: '',
    scope: 0, //使用范围 0: 所有员工可填写 2： 管理员填写 3: 员工填写
    state: 0, // 开启状态 0: 禁用 1： 启用
    type: 0, //1： 基本标签 2： 自定义标签
    userId: 0
};
BaseContactInfo = {
    userId: 0,
    displayName: '',
    pinyin: '',
    localAvatar: '',
    remoteAvatarUrl: '',
    personalSign: '',
    departmentId: 0,
    departmentName : '',
    position: ''
};
MessageInfo = {
    msgId: 0,
    msgSeq: 0,
    msgTime: 0,
    version: 0,
    appId: 0,
    protocolType: 0,
    protocolId:0,
    from: {},
    to: {},
    conversation: 0,
    isPlayed: 0, //多媒体是否已播放 1:已播放/0:未播放
    sendState: 0,
    msgData: '',
    conversationType: 1,
    relateId: 0,
    flag: 0
};
GroupInfo = {
    id: 0,
    siteId: 0,
    name: '',
    pinyin: '',
    avatarIndex: '',
    localAvatar: '',
    joinState: 0,
    memberCount: 0,
    adminId: 0,
    alertState: 0,
    alertRule: 0,
    alertTime: 0,
    topState: 0,
    status: 0,
    disturbState: 0,
    nameFlag: 0,
    groupType: 1,
    logoLevel: 0,
    members: []
};
GroupMemberInfo = {
    userId: 0,
    joinTime: 0
};
MeetingInfo = {
    conferenceId: 0,
    eventId: 0,
    userId: 0,
    authorizerId: 0,
    hostId: 0,
    hostName: '',
    hostPassword: '',
    attendees: [],
    attendeePassword: '',
    title: '',
    summary: '',
    location: '',
    timeZone: '',
    billingCode: 0,
    startTime: 0,
    duration: 0,
    category: 0,
    isGNet: 0,
    language: 0,
    emailLanguage: 0,
    isCycle: 0,
    repeatCount: 0,
    repeatEndData: 0,
    isOld: 0
};
Attendee = {
    account: '',
    type: 0, //参会人类型：0 内部用户， 1 邮件用户， 2 电话用户
    status: 0, //参会人状态：0 拒绝， 1 接受， 2 未处理
    startTime: 0,
    name: 0
};
DepartmentInfo = {
    id: 0,
    departmentName: '',
    parentId: 0, // 父部门Id
    staffNumber: 0, //指数员工数量
    totalNumber: 0, //所有员工数量
    onlineNumber: 0,
    staffs: [], //员工信息列表， 参见ContactInfo
    subDepartments: [] //子部门列表
};
AppInfo = {
    appId: 0,
    appTitle: '',
    appDesc: '',
    appLogo: '',
    pcUrl: '',
    iosUrl: '',
    androidUrl: ''
};