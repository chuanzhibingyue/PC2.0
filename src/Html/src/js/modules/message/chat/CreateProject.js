/**
 *  该文件已废弃 —— by xue.bai_2 on 2016/05/25
 *
 *  Created by xue.bai_2 on 2015/08/26
 */
(function(uc){
	// forbidden right-hand button
	jQuery(document.body).contextmenu(function(e){ e.preventDefault(); });
	
	 uc.modules.chat.CreateProject = {
		init: function(){
			uc.modules.component.TitleBarButton.init();
			this._bindEvent();
		},
		_bindEvent: function(){
			// show or hide save button
			$(".project-name").keyup(function(){
				if($(".project-name").val() == ''){
					$(".project-save").find("a").eq(1).removeClass("save").addClass("disabled");
				}else{
					$(".project-save").find("a").eq(1).removeClass("disabled").addClass("save");
				}
			});
			
			// click the cancel button
			$(".project-save").off("click");
			$(".project-save").on("click", ".cancel", function(){
				uc.util.WindowUtil.close();
			});
			
			// click the save button
			$(".project-save").on("click", ".save", function(){
				var projectName = $(".project-name").val();
				console.log("projectName=" + projectName);
				
				if(!projectName){
					return;
				}
				
				uc.IUIService.triggerCrossWindowEvent('CreateProjectName', {name:projectName});
				
				uc.util.WindowUtil.close();
			});
			
			$(".project-name").keydown(function(e){
				if(e.keyCode == 13){
					var projectName = $(".project-name").val();
					console.log("projectName=" + projectName);
					
					if(!projectName){
						return;
					}
					
					uc.IUIService.triggerCrossWindowEvent('CreateProjectName', {name:projectName});
					
					uc.util.WindowUtil.close();
				}
			});
		}
	 };
	 jQuery(document).ready(function(){
		uc.init();
		uc.modules.chat.CreateProject.init();
	 });
}(uc));