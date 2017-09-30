(function (uc) {	
    uc.modules.component.PromptMessageBox = (function(){
        var	 
        method = {},
            returnValue,
            $overlay,
            $content,
            $title,
            $message,
            $close,	
            
            $buttonMode,
            $ok,	
            $cancel,
            
            $okFunc,
            $cancelFunc,
            
            $icon,
            $status,		
            
            $modal;			
        
        $overlay = $('<div id="modOverlay"></div>');
        $modal = $('<div id="modalWin" class="ui-body-c"></div>');
        $title = $('<p class="title"></p>')
        $message = $('<p></p>')	
        $content = $('<div id="modContent"></div>');
        $close = $('<button id="clsBtn" class="btnCls">X</button>');
        //$icon = $('<div class="icon"></div>')
        
        $ok = $('<button id="ok" class="btn btn-ok">OK</button>')
        $cancel = $('<button id="cancel" class="btn btn-cancel">Cancel</button>')
        
        $content.append($title, $message);
        
        $modal.hide();
        $overlay.hide();
        
        method.cancelDialog = function(){
        	method.close();
        };
        
        method.center = function () {
            var top, left;
            
            top = Math.max($(window).height() - $modal.outerHeight(), 0) / 2;
            left = Math.max($(window).width() - $modal.outerWidth(), 0) / 2;
            
            $modal.css({
                top:top + $(window).scrollTop(), 
                left:left + $(window).scrollLeft()
            });
        };
        
        method.open = function (settings, callback) {
            $cancel.add($ok).hide();
            $title.empty().append(settings.title);
            $message.empty().append(settings.message);
            $status = settings.status;
            retValue = settings.returnValue;
            
            method.retrnFunc = callback;
            
            if(settings.OkText){
            	$ok.text(settings.OkText);
            }
            
            if(settings.CancelText){
            	$cancel.text(settings.CancelText);
            }

            $ok.removeClass('btn-green');
            $ok.removeClass('btn-blue');
            $ok.removeClass('btn-red');
            $ok.removeClass('btn-yellow');
            $ok.removeClass('btn-light-blue');
            switch(settings.status)
            {
                case 'Green':
                    $ok.addClass('btn-green');
                    break;
                case 'Blue':
                    $ok.addClass('btn-blue');
                    break;
                case 'Red':
                    $ok.addClass('btn-red');
                    break;
                case 'Yellow':
                    $ok.addClass('btn-yellow');
                    break;
                case 'Light-Blue':
                    $ok.addClass('btn-light-blue');
                    break;
                default :
                    $ok.addClass('btn-blue');
            }
            switch(settings.buttonMode)
            {
                case 'ok':
                    $ok.show();
                    $modal.append($content, $close, $ok);
                    break;
                case 'cancel':
                    $cancel.show();
                    $modal.append($content, $close, $cancel);	  
                    break;
                case 'both':
                    $cancel.add($ok).show();
                    $modal.append($content, $close, $ok, $cancel);		 
                    break; 
                default:
                    $cancel.add($ok).show();
                    $modal.append($content, $close, $ok, $cancel);		  
            }		
            
            $modal.css({
                width: settings.width || 'auto', 
                height: settings.height || 'auto'			
            });
            
            method.center();
            $modal.show();
            $overlay.show();
        };
        
        method.close = function () {
            $modal.hide();
            $overlay.hide();
            $title.empty();
            $message.empty();
        };
        
        method.retrnFunc = function (i) {
            retValue = i;		
           // alert(retValue);
        };	
        
        $close.click(function(e){
            e.preventDefault();
            method.close();
        });
        
        $ok.click(function(e){			
            e.preventDefault();	
            method.retrnFunc(0);				
        });
        
        $cancel.click(function(e){			
            e.preventDefault();		
            method.retrnFunc(1);						
        });
        
        $(document).ready(function(){
            $('body').append($overlay, $modal);						
        });
        
        return method;
    }());
})(uc);




