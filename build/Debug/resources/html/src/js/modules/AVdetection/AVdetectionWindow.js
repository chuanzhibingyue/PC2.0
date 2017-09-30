/**
 * Created by hougang.chen on 2016/1/8
 */
(function (uc) {
	$(document).keyup(function (e) {
		if (e.keyCode == 27) {
			uc.util.WindowUtil.close();
		}
	});
	uc.modules.AVdetection.AVdetectionWindow = {
		init: function () {
			this.nextStepBtn = $('.stepBtnContent .btn-nextstep');
			this.prevStepBtn = $('.stepBtnContent .btn-prevstep');
			this.closeBtn = $('.tool-icon-container .icon-close');
			this.AVdetectionWindow = $('.AVdetectionWindow');
			this.stepShowContent = $('.stepShowContent', this.AVdetectionWindow);
			this.stepone = $('.stepone', this.stepShowContent);
			this.steptwo = $('.steptwo', this.stepShowContent);
			this.stepthree = $('.stepthree', this.stepShowContent);
			this.speakerContent =  $('.speakerContent', this.AVdetectionWindow);
			this.mikeContent =  $('.mikeContent', this.AVdetectionWindow);
			this.vedioContent =  $('.vedioContent', this.AVdetectionWindow);
			this.mikeList = $('.mikeContent .mikeList');
			this.speakerList = $('.speakerContent .speakerList');
			this.mikeHardwareSelect = $('.mikeContent .hardwareSelect');
			this.speakerHardwareSelect = $('.speakerContent .hardwareSelect');
			this.speakerTurnOn=$('.speakerContent .btn-turnOn');
			this.detectionVideoOn=$('.speakerContent .detectionVideoOn ');
			this.mikeDetectionOn=$('.mikeContent .detectionMikeOn ');
			this.mikeclickDetectionBtn=$('.mikeContent .clickDetectionBtn');
			this.maxVideo=32768;
			this.bindEvent();
			this.showSpeakingDom();
		},
		getSelectedDetection:function(detectionType){
			var speakerList;
			switch(detectionType){
				case "speaking":
					speakerList=uc.ICallService.getSpeakerList(function(arg){
						if(arg[0] == uc.ErrorCode.Success){

						}
					});
					speakerList=$.parseJSON(speakerList);
					this.setEquipmentList(detectionType,speakerList);
					break;
				case "mike":
					speakerList=uc.ICallService.getMicrophoneList(function(arg){
						if(arg[0] == uc.ErrorCode.Success){

						}
					});
					speakerList=$.parseJSON(speakerList);
					this.setEquipmentList(detectionType,speakerList);
					break;
				case "radio":
					break;
			}
		},
		setEquipmentStatus:function(detectionType,speakerName,Level){
			switch(detectionType){
				case "speaking":
					if(!this.speakerContent.hasClass("hide")){
						if(this.speakerHardwareSelect.html() == speakerName){
							this.detectionVideoOn.width((Level/this.maxVideo)*258+"px");
						}
					}
					break;
				case "mike":
					if(!this.mikeContent.hasClass("hide")){
						if(this.mikeHardwareSelect.html() == speakerName){
							this.mikeDetectionOn.width((Level/this.maxVideo)*100+"%");
						}
					}
					break;
				case "radio":
					break;
			}
		},
		setEquipmentList:function(detectionType,list){
			var html='';
			if(list.length){
				for(var i=0;i<list.length;i++){
					html=html+'<li>'+list[i]+'</li>'
				}
			}else{
				html=html+'<li>'+uc_resource.AVdetection.detectionEquipment+'</li>'
			}
			switch(detectionType){
				case "speaking":
					this.speakerList.html(html);
					this.speakerList.find("li:first").trigger("click");
					break;
				case "mike":
					this.mikeList.html(html);
					this.mikeList.find("li:first").trigger("click");
					break;
				case "radio":
					break;
			}

		},
		stopDiagnosis:function(detectionType,audio,notStart){
			var _this=this;
			switch(detectionType){
				case "speaking":
					uc.ICallService.stopSpeakerDiagnosis(function(arg){
						if(arg[0] == uc.ErrorCode.Success){
							_this.detectionVideoOn.width("0px");
							if(!notStart){
								_this.startDiagnosis(detectionType,audio);
								_this.speakerTurnOn.html(uc_resource.AVdetection.speakeContent.StopDetectionBtn);
								_this.speakerTurnOn.addClass("turn-off");
							}else{
								_this.speakerTurnOn.html(uc_resource.AVdetection.speakeContent.DetectionBtn);
								_this.speakerTurnOn.removeClass("turn-off");
							}
						}
					});
					break;
				case "mike":
					uc.ICallService.stopMikeDiagnosis(function(arg){
						if(arg[0] == uc.ErrorCode.Success){
							_this.mikeDetectionOn.width("0px");
							if(!notStart){
								_this.startDiagnosis(detectionType,audio);
							}
						}
					});
					break;
				case "radio":
					break;
			}
		},
		startDiagnosis:function(detectionType,audio){
			switch(detectionType){
				case "speaking":
					uc.ICallService.startSpeakerDiagnosis(audio,function(arg){
						if(arg[0] == uc.ErrorCode.Success){

						}
					});
					break;
				case "mike":
					uc.ICallService.startMikeDiagnosis(audio,function(arg){
						if(arg[0] == uc.ErrorCode.Success){

						}
					});
					break;
				case "radio":
					break;
			}
		},
		showSpeakingDom:function(){
			if(this.mikeHardwareSelect.html()){
				this.stopDiagnosis("mike", this.mikeHardwareSelect.html(),true);
			}
			this.stepShowContent.find("li.ui-step-done").removeClass("ui-step-done");
			this.stepone.addClass("ui-step-done");
			this.AVdetectionWindow.find(".hide").removeClass("hide");
			this.mikeContent.addClass("hide");
			this.vedioContent.addClass("hide");
			this.showType="speaking";
			this.prevStepBtn.hide();
			this.nextStepBtn.html(uc_resource.AVdetection.stepbutton.nextStepBtn);
			if(!this.speakerList.find("li").length){
				this.getSelectedDetection(this.showType);
			}

		},
		showMikeDom:function(){
			this.stopDiagnosis("speaking", this.speakerHardwareSelect.html(),true);
			this.stepShowContent.find("li.ui-step-done").removeClass("ui-step-done");
			this.steptwo.addClass("ui-step-done");
			this.AVdetectionWindow.find(".hide").removeClass("hide");
			this.speakerContent.addClass("hide");
			this.vedioContent.addClass("hide");
			this.showType="mike";
			this.prevStepBtn.show();
			//this.nextStepBtn.html(uc_resource.AVdetection.stepbutton.nextStepBtn);
			this.nextStepBtn.html(uc_resource.AVdetection.stepbutton.finishBtn);
			if(!this.mikeList.find("li").length){
				this.getSelectedDetection(this.showType);
			}
			this.startDiagnosis("mike", this.mikeHardwareSelect.html());

		},
		showRadioDom:function(){
			this.stopDiagnosis("mike", this.mikeHardwareSelect.html(),true);
			this.stepShowContent.find("li.ui-step-done").removeClass("ui-step-done");
			this.stepthree.addClass("ui-step-done");
			this.AVdetectionWindow.find(".hide").removeClass("hide");
			this.mikeContent.addClass("hide");
			this.speakerContent.addClass("hide");
			this.showType="radio";
			this.prevStepBtn.show();
			this.nextStepBtn.html(uc_resource.AVdetection.stepbutton.finishBtn);
			this.getSelectedDetection(this.showType);
		},
		diagnosisFinished:function(speakerName){
			if(this.speakerHardwareSelect.html() == speakerName){
				this.speakerTurnOn.html(uc_resource.AVdetection.speakeContent.DetectionBtn);
				this.speakerTurnOn.removeClass("turn-off");
				this.detectionVideoOn.width("0px");
			}
		},
		bindEvent: function () {
			var self = this;
			this.closeBtn.on('click', function () {
				self.stopDiagnosis("speaking",null,true);
				self.stopDiagnosis("mike",null,true);
				uc.util.WindowUtil.close();
			});
			this.mikeHardwareSelect.click(function(){
				self.mikeList.show();
			});
			this.speakerHardwareSelect.click(function(){
				self.speakerList.show();
			});
			this.speakerList.on('click',"li", function () {
				var selected=$(this).text();
				self.speakerHardwareSelect.html(selected);
				self.speakerList.hide();
				self.detectionVideoOn.width("0px");
			});
			this.mikeList.on('click','li', function () {
				var selected=$(this).text();
				self.mikeHardwareSelect.html(selected);
				self.mikeList.hide();
				self.stopDiagnosis("mike",selected);
				self.mikeDetectionOn.width("0px");
			});

			this.speakerTurnOn.click(function(){
				if($(this).hasClass("turn-off")){
					var audio=self.speakerHardwareSelect.html();
					self.stopDiagnosis("speaking",audio,true);
				}else{
					var audio=self.speakerHardwareSelect.html();
					self.stopDiagnosis("speaking",audio);
				}
			});

			this.nextStepBtn.click(function(){
				switch(self.showType){
					case "speaking":
						self.showMikeDom();
						break;
					case "mike":
						//self.showRadioDom();
						self.closeBtn.trigger("click");
						break;
					case "radio":
						self.closeBtn.trigger("click");
						break;
				}
			});

			this.prevStepBtn.click(function(){
				switch(self.showType){
					case "speaking":
						break;
					case "mike":
						self.showSpeakingDom();
						break;
					case "radio":
						self.showMikeDom();
						break;
				}
			});

			$(document).click(function(e){
				if(!$(e.target).hasClass("hardwareSelect") &&
					!$(e.target).hasClass("mikeList") &&
					!$(e.target).hasClass("speakerList")){
					self.mikeList.hide();
					self.speakerList.hide();
				}
			});
		}
	};
	jQuery(document).ready(function () {
		uc.init();
		uc.modules.AVdetection.AVdetectionWindow.init();
	});
})(uc);