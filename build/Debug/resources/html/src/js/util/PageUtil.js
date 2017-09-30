/**
 * Created by waouyang on 15/8/15.
 */
(function(uc){
    var nodeFuncArray = [], moduleClickObj = {};
    uc.util.PageUtil = {
        /**
         * add node which needs to click
         * @param node
         * @param nodeFunc
         * @param bodyFunc
         */
        addClickBodyHideNode: function(node, nodeFunc, bodyFunc){
            var nodeFuncObj = {
                node: node,
                nodeFunc: nodeFunc,
                bodyFunc: bodyFunc
            };
            nodeFuncArray.push(nodeFuncObj);
        },
        addBodyClick: function (name, node, bodyFunc) {
            if (name) {
                moduleClickObj[name] = {
                    node: node,
                    bodyFunc: bodyFunc
                }
            }
        },
        removeBodyClick: function (name) {
            moduleClickObj[name] = '';
        },
        getModuleClick: function () {
            return moduleClickObj;
        }
    };
    jQuery(document).ready(function(){
        jQuery(document).click(function(e){
            var nodeFuncObj;
            for(var i = 0, len = nodeFuncArray.length; i < len; i++){
                nodeFuncObj = nodeFuncArray[i];
                if(!e.target.parentNode){
                    continue;
                }
                if(e.target == nodeFuncObj.node || $.contains(nodeFuncObj.node, e.target)){
                    nodeFuncObj.nodeFunc && nodeFuncObj.nodeFunc();
                }else{
                    nodeFuncObj.bodyFunc && nodeFuncObj.bodyFunc();
                }
            }

            for (var event in moduleClickObj) {
                nodeFuncObj = moduleClickObj[event];
                if(!e.target.parentNode){
                    continue;
                }
                if(e.target != nodeFuncObj.node || !$.contains(nodeFuncObj.node, e.target)){
                    nodeFuncObj.bodyFunc && nodeFuncObj.bodyFunc(e);
                }
            }
        });
    })
})(uc);