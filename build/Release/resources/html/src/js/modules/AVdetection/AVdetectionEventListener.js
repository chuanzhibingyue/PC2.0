/**
 * Created with JetBrains WebStorm.
 * User: hougang.chen
 * Date: 16-1-12
 * Time: 下午2:17
 * To change this template use File | Settings | File Templates.
 */
(function (uc) {
	uc.modules.AVdetection.AVdetectionEventListener = function () {
	}
	jQuery.extend(uc.modules.AVdetection.AVdetectionEventListener.prototype,
		uc.interfaces.IUIEvent,
        uc.interfaces.ICallEvent,
		{
            OnGetSpeakerList:function(errorCode,speakers){
                uc.modules.AVdetection.AVdetectionManager.OnGetSpeakerList(errorCode,speakers);
            },
            OnGetMicrophoneList:function(errorCode,microphone){
                uc.modules.AVdetection.AVdetectionManager.OnGetMicrophoneList(errorCode,microphone);
            },
            OnMikeDiagnosisStatus:function(mikeName,mikeLevel){
                uc.modules.AVdetection.AVdetectionManager.OnMikeDiagnosisStatus(mikeName,mikeLevel);
            },
            OnSpeakerDiagnosisStatus:function(speakerName,speakerLevel){
                uc.modules.AVdetection.AVdetectionManager.OnSpeakerDiagnosisStatus(speakerName,speakerLevel);
            },
            OnSpeakerDiagnosisStop:function(speakerName){
                uc.modules.AVdetection.AVdetectionManager.OnSpeakerDiagnosisStop(speakerName);
            }
		});
})(uc);