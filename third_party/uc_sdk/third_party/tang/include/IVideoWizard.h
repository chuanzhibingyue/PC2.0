#ifndef IVIDEOWIZARD_H
#define IVIDEOWIZARD_H

#ifdef AVWIZARD_EXPORTS
#define VIDEOWIZARD_EXPORTS extern "C" __declspec(dllexport)
#else
#define VIDEOWIZARD_EXPORTS extern "C" __declspec(dllimport)
#endif //AVWIZARD_EXPORTS

class IVideoWizard
{
public:
	virtual int GetCameraNum(int& nCount) = 0;
	virtual int GetCameraName(int nIndex, char* szDeviceName) = 0;
	virtual int IsCameraAvailable(bool& bCamAvailable) = 0;
	virtual int SetDefaultCamera(int nIndex) = 0;
	virtual int GetDefaultCamera(int& nIndex) = 0;
	virtual int StartPreview(int nIndex, void *pWindow, int nWindowType) = 0;
	virtual int StopPreview() = 0;
};

VIDEOWIZARD_EXPORTS IVideoWizard* createVideoWizard();
VIDEOWIZARD_EXPORTS int deleteVideoWizard(IVideoWizard* pVideoWizard);

#endif //IVIDEOWIZARD_H