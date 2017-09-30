/**
 * Created by zhen.xu on 2015/9/18.
 */
(function (uc) {
    uc.modules.microblog.MicroblogManager = {
        OnInitClient: function (flag) {
            uc.modules.microblog.Microblog.init(flag);
        }
    };
})(uc);