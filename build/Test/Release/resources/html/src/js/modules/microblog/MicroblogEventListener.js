/**
 * Created by zhen.xu on 2015/9/18.
 */

(function (uc) {
    uc.modules.microblog.MicroblogEventListener = function () {

    };

    jQuery.extend(uc.modules.microblog.MicroblogEventListener.prototype,
        uc.interfaces.IUIEvent,
        uc.interfaces.IMicroblogEvent, {
        OnInitClient: function(clearData){
            uc.modules.microblog.MicroblogManager.OnInitClient(clearData);
        },
        OnNewMsgCountGet: function (errorCode, countAll, relationCount, timenewsCount, pollCount, caseCount) {
            uc.modules.microblog.Microblog.OnNewMsgCountGet(errorCode, countAll, relationCount, timenewsCount, pollCount, caseCount);
        },
        OnAttentionAdd: function (errorCode, userId, relation) {},
        OnAttentionCancelled: function (errorCode, userId, relation) {},
        OnOpenMyLinkPage: function () {
            uc.modules.microblog.Microblog.OnOpenMyLinkPage();
        }
    });
})(uc);