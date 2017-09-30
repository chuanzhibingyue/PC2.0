#pragma once
#include <MMReg.h>
#include <MMSystem.h>

struct WaveData
{
    WAVEFORMATEX    wfmtx;
    LPSTR           pData;
    DWORD           dwSize;         // size in bytes
    unsigned int    bDecodeFinished; // decode finished or not
};

struct WaveBlock
{
    /// wave block
    unsigned long   wpos;         /// current play position
    WAVEHDR*        pWaveHdr;     /// wave block
    volatile int    wfreeblock;   /// free blocks left
    int             wcurrblock;   /// current block
};