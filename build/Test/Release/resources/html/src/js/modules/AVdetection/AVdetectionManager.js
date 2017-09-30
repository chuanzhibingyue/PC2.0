/**
 * Created with JetBrains WebStorm.
 * User: hougang.chen
 * Date: 16-1-12
 * Time: 下午2:18
 * To change this template use File | Settings | File Templates.
 */

(function (uc) {
	uc.modules.AVdetection.AVdetectionManager = {
        OnGetSpeakerList:function(errorCode,speakers){
            if(errorCode == uc.ErrorCode.Success){
                uc.modules.AVdetection.AVdetectionWindow.setEquipmentList("speaking",speakers);
            }else{
                uc.modules.component.PromptMessageBox.open({
                    title: uc_resource.WindowTitle.Notification,
                    message: uc_resource.Setting.GetDetectionFailed,
                    buttonMode: "ok",
                    OkText: uc_resource.Common.OK
                }, function () {
                    this.close();
                });
            }
        },
        OnGetMicrophoneList:function(errorCode,microphone){
            if(errorCode == uc.ErrorCode.Success){
                uc.modules.AVdetection.AVdetectionWindow.setEquipmentList("mike",microphone);
            }else{
                uc.modules.component.PromptMessageBox.open({
                    title: uc_resource.WindowTitle.Notification,
                    message: uc_resource.Setting.GetDetectionFailed,
                    buttonMode: "ok",
                    OkText: uc_resource.Common.OK
                }, function () {
                    this.close();
                });
            }
        },
        OnSpeakerDiagnosisStatus:function(speakerName,speakerLevel){
            uc.modules.AVdetection.AVdetectionWindow.setEquipmentStatus("speaking",speakerName,speakerLevel);
        },
        OnMikeDiagnosisStatus:function(mikeName,mikeLevel){
            uc.modules.AVdetection.AVdetectionWindow.setEquipmentStatus('mike',mikeName,mikeLevel);
        },
        OnSpeakerDiagnosisStop:function(speakerName){
            uc.modules.AVdetection.AVdetectionWindow.diagnosisFinished(speakerName);
        }
	}
})(uc);
