/**
 * Created by zhen.xu on 2015/9/18.
 */
(function (uc) {
    uc.interfaces.IMicroblogEvent = {
        OnNewMsgCountGet: function (errorCode, countAll, relationCount, timenewsCount, pollCount, caseCount) {},
        OnAttentionAdd: function (errorCode, userId, relation) {},
        OnAttentionCancelled: function (errorCode, userId, relation) {}
    };
})(uc);