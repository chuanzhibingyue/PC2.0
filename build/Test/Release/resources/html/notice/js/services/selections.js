appModule.service('selection', function(){
  return{
    setCursorEnd:function(el) {
        el.focus();
        var range = document.createRange();
        range.selectNodeContents(el);
        range.collapse(false);
        var sel = window.getSelection();
        sel.removeAllRanges();
        sel.addRange(range);
    },
    saveSelection:function() {
        if (window.getSelection) {
            var sel = window.getSelection();
            if (sel.getRangeAt && sel.rangeCount) {
                var ranges = [];
                for (var i = 0, len = sel.rangeCount; i < len; ++i) {
                    ranges.push(sel.getRangeAt(i));
                }
                return ranges;
            }
        } else if (document.selection && document.selection.createRange) {
            return document.selection.createRange();
        }
        return null;
    },
    restoreSelection:function(savedSel) {
    if (savedSel) {
        if (window.getSelection) {
            var sel = window.getSelection();
            sel.removeAllRanges();
            for (var i = 0, len = savedSel.length; i < len; ++i) {
                sel.addRange(savedSel[i]);
            }
        } else if (document.selection && savedSel.select) {
            savedSel.select();
        }
    }
    },
    clearSelection:function(){
        var sel = window.getSelection();
        // sel.removeAllRanges();
        sel.collapseToEnd();
    },
    getSelectionParent:function(selection,name){
        if(!selection || !selection.focusNode){
            return null;
        }

        if(selection.focusNode && selection.focusNode.nodeType == 1){
            return $(selection.focusNode).closest(name)[0];
        }

        if(selection.focusNode){
            return $(selection.focusNode.parentElement).closest(name)[0];
        }

        return null;
    },
    changeSelectionToElement:function(selection,element){
        var range = document.createRange();
        range.selectNode(element);
        selection.removeAllRanges();
        selection.addRange(range);
    }
  };
});
