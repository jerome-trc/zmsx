/// @file
/// @brief C++ header of symbols common to several source files.

#pragma once
#define ZMSX_HPP

#if defined(_MSC_VER) && !defined(ZMSX_STATIC)
#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllexport)	// without this the compiler complains.
#else
#define DLL_EXPORT
#define DLL_IMPORT
#endif

typedef class MIDISource ZMSXMidiSource;
typedef class MusInfo ZMSXMusicStream;

// Build two configurations - lite and full.
// Lite only uses FluidSynth for MIDI playback and is licensed under the LGPL v2.1
// Full uses all MIDI synths and is licensed under the GPL v3.

#ifndef ZMSX_LITE
#define HAVE_GUS		// legally viable but not really useful
#define HAVE_TIMIDITY	// GPL v2.0
#define HAVE_OPL		// GPL v3.0
#define HAVE_ADL		// GPL v3.0
#define HAVE_OPN		// GPL v3.0
#define HAVE_WILDMIDI	// LGPL v3.0
#endif

#include "zmsx.h"
#include "fileio.h"

void SetError(const char *text);

struct CustomFileReader : public MusicIO::FileInterface
{
	ZMSXCustomReader* cr;

	CustomFileReader(ZMSXCustomReader* zr) : cr(zr) {}
	virtual char* gets(char* buff, int n) { return cr->gets(cr, buff, n); }
	virtual long read(void* buff, int32_t size) { return cr->read(cr, buff, size); }
	virtual long seek(long offset, int whence) { return cr->seek(cr, offset, whence); }
	virtual long tell() { return cr->tell(cr); }
	virtual void close()
	{
		cr->close(cr);
		delete this;
	}

};

void ZMusic_Printf(int type, const char* msg, ...);

inline uint8_t ZMusic_SampleTypeSize(ZMSXSampleType stype)
{
    switch(stype)
    {
    case zmsx_sample_uint8: return sizeof(uint8_t);
    case zmsx_sample_int16: return sizeof(int16_t);
    case zmsx_sample_float32: return sizeof(float);
    }
    return 0;
}

inline uint8_t ZMusic_ChannelCount(ZMSXChannelConfig chans)
{
    switch(chans)
    {
    case zmsx_chancfg_mono: return 1;
    case zmsx_chancfg_stereo: return 2;
    }
    return 0;
}

inline const char *ZMusic_ChannelConfigName(ZMSXChannelConfig chans)
{
    switch(chans)
    {
    case zmsx_chancfg_mono: return "Mono";
    case zmsx_chancfg_stereo: return "Stereo";
    }
    return "(unknown)";
}
