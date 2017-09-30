#pragma once

#include "ISoundControl.h"

class MixerSoundControl :
	public ISoundControl
{
public:
	MixerSoundControl(void);
public:
	virtual ~MixerSoundControl(void);

public:
	HRESULT SetOutVolume(DWORD dwOutVolume);					/**< 设置播放音量值 */	
	HRESULT IsSupportRecordInAndOutSound(UINT32 & bSupport);		/**< 是否支持同时录制麦克风和扬声器的声音 */
	HRESULT EnableStereoMix();									/**< 启用立体声混音 */
	HRESULT DisableStereoMix();									/**< 禁用立体声混音 */
};
