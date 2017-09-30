/**
 * Created by waouyang on 15/12/16.
 */
(function(uc){
    var inputting = false;
    var cursorLeft, cursorTop, position;
    uc.util.CaretUtil = {
        /**
         * set iframe editor caret position
         * @param iframeBody    iframe editor document body
         * @param iframeDOM     iframe DOM node, not jQuery object
         */
        setIframeEditorCaretPosition: function(iframeBody, iframeDOM){
            $(iframeBody).on('keydown mousedown', function(){
                if(isNaN(cursorLeft) || isNaN(cursorTop)){
                    cursorLeft = $(iframeDOM).offset().left + 5;
                    cursorTop = $(iframeDOM).offset().top + 10;
                }
                uc.util.CaretUtil._setCaretPosition(cursorLeft + 10, cursorTop + 8);
            });
            $(iframeBody).on('keyup mouseup', function(){
                position = $(iframeBody).caret('position', {iframe: iframeDOM});

                cursorTop = $(iframeDOM).offset().top + position.top;
                cursorLeft = $(iframeDOM).offset().left + position.left;
            });
        },

        /**
         * set input/textarea/div caret position
         * @param inputNode input jQuery node
         */
        setInputEditorCaretPosition: function(inputNode){
            inputNode.on('keyup mouseup', function(){
                position = inputNode.caret('position');
                cursorTop = inputNode.offset().top + position.top;
                cursorLeft = inputNode.offset().left + position.left;
            });
            inputNode.on('keydown mousedown', function(){
                if(isNaN(cursorLeft) || isNaN(cursorTop)){
                    cursorLeft = inputNode.offset().left + 5;
                    cursorTop = inputNode.offset().top + 10;
                }
                uc.util.CaretUtil._setCaretPosition(cursorLeft + 10, cursorTop + 8);
            })
        },
        _setCaretPosition: function(left, top){
            if(inputting){
                return;
            }
            setTimeout(function(){
                inputting = false;
            }, 300);
            //console.log('cursorLeft = ' + left + ', cursorTop =  ' + top);
            uc.util.SystemUtil.setInputMethodCaretPosition(left, top);
            inputting = true;
        }
    }
})(uc);