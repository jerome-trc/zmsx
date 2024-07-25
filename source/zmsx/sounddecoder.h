#pragma once

#include "zmsx.hpp"
#include <vector>

struct SoundDecoder
{
	static SoundDecoder* zmsx_create_decoder(MusicIO::FileInterface* reader);

    virtual void getInfo(int *samplerate, zmsx_ChannelConfig *chans, zmsx_SampleType *type) = 0;

    virtual size_t read(char *buffer, size_t bytes) = 0;
    virtual std::vector<uint8_t> readAll();
    virtual bool seek(size_t ms_offset, bool ms, bool mayrestart) = 0;
    virtual size_t getSampleOffset() = 0;
    virtual size_t getSampleLength() { return 0; }
	virtual bool open(MusicIO::FileInterface* reader) = 0;

    SoundDecoder() { }
    virtual ~SoundDecoder() { }

protected:
    friend class SoundRenderer;

    // Make non-copyable
    SoundDecoder(const SoundDecoder &rhs) = delete;
    SoundDecoder& operator=(const SoundDecoder &rhs) = delete;
};
