#include "LogManager.h"


class IGNetTangConference;
class IGNetTangConferenceSink;
class IGNetTangService;

typedef IGNetTangConference* (*CREATE_CONFERENCE_FUN)(const char*, IGNetTangConferenceSink*);
typedef int (*DELETE_CONFERENCE_FUN)(IGNetTangConference* confInstance);
typedef IGNetTangService* (*CREATE_TANG_SERVICE_FUN)();