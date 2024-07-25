#pragma once

// Anything streamed to the sound system as raw wave data, except MIDIs --------------------

#include <stdlib.h>
#include "zmsx/mididefs.h"	// for StreamSourceInfo
#include "zmsx/midiconfig.h"

class StreamSource
{
protected:
	bool m_Looping = true;
	int m_OutputRate;

public:

	StreamSource (int outputRate) { m_OutputRate = outputRate; }
	virtual ~StreamSource () {}
	virtual void SetPlayMode(bool looping) { m_Looping = looping; }
	virtual bool Start() { return true; }
	virtual bool SetPosition(unsigned position) { return false; }
	virtual bool SetSubsong(int subsong) { return false; }
	virtual bool GetData(void *buffer, size_t len) = 0;
	virtual zmsx_SoundStreamInfoEx GetFormatEx() = 0;
	virtual std::string GetStats() { return ""; }
	virtual void ChangeSettingInt(const char *name, int value) {  }
	virtual void ChangeSettingNum(const char *name, double value) {  }
	virtual void ChangeSettingString(const char *name, const char *value) {  }

protected:
	StreamSource() = default;
};


StreamSource *MOD_OpenSong(MusicIO::FileInterface* reader, int samplerate);
StreamSource* GME_OpenSong(MusicIO::FileInterface* reader, const char* fmt, int sample_rate);
StreamSource *SndFile_OpenSong(MusicIO::FileInterface* fr);
StreamSource* XA_OpenSong(MusicIO::FileInterface* reader);
StreamSource* OPL_OpenSong(MusicIO::FileInterface* reader, OPLConfig *config);
