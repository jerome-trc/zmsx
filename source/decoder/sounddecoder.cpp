/*
** sounddecoder.cpp
** baseclass for sound format decoders
**
**---------------------------------------------------------------------------
** Copyright 2008-2019 Chris Robinson
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**---------------------------------------------------------------------------
**
*/


#include "zmsx/zmsx.hpp"
#include "sndfile_decoder.h"
#include "mpg123_decoder.h"

SoundDecoder *SoundDecoder::zmsx_create_decoder(MusicIO::FileInterface *reader)
{
    SoundDecoder *decoder = NULL;
    auto pos = reader->tell();

#ifdef HAVE_SNDFILE
		decoder = new SndFileDecoder;
		if (decoder->open(reader))
			return decoder;
		reader->seek(pos, SEEK_SET);

		delete decoder;
		decoder = NULL;
#endif
#ifdef HAVE_MPG123
		decoder = new MPG123Decoder;
		if (decoder->open(reader))
			return decoder;
		reader->seek(pos, SEEK_SET);

		delete decoder;
		decoder = NULL;
#endif
    return decoder;
}


// Default readAll implementation, for decoders that can't do anything better
std::vector<uint8_t> SoundDecoder::readAll()
{
    std::vector<uint8_t> output;
    unsigned total = 0;
    unsigned got;

    output.resize(total+32768);
    while((got=(unsigned)read((char*)&output[total], output.size()-total)) > 0)
    {
        total += got;
        output.resize(total*2);
    }
    output.resize(total);
    return output;
}

//==========================================================================
//
// other callbacks
//
//==========================================================================
extern "C"
short* dumb_decode_vorbis(int outlen, const void* oggstream, int sizebytes)
{
	short* samples = (short*)calloc(1, outlen);
	zmsx_ChannelConfig chans;
	zmsx_SampleType type;
	int srate;

	// The decoder will take ownership of the reader if it succeeds so this may not be a local variable.
	MusicIO::MemoryReader* reader = new MusicIO::MemoryReader((const uint8_t*)oggstream, sizebytes);

	SoundDecoder* decoder = SoundDecoder::zmsx_create_decoder(reader);
	if (!decoder)
	{
		reader->close();
		return samples;
	}

	decoder->getInfo(&srate, &chans, &type);
	if (chans != ChannelConfig_Mono)
	{
		delete decoder;
		return samples;
	}

	if(type == SampleType_Int16)
		decoder->read((char*)samples, outlen);
	else if(type == SampleType_Float32)
	{
		constexpr size_t tempsize = 1024;
		float temp[tempsize];
		size_t spos = 0;

		outlen /= sizeof(short);
		int done = 0;
		while(done < outlen)
		{
			size_t got = decoder->read((char*)temp, tempsize * sizeof(float)) / sizeof(float);
			for(size_t i = 0;i < got;++i)
			{
				float s = temp[i] * 32768.0f;
				samples[spos++] = (s > 32767.0f) ? 32767 : (s < -32768.0f) ? -32768 : (short)s;
			}
			if(got < tempsize)
				break;
			done += got;
		}
	}
	else if(type == SampleType_UInt8)
	{
		constexpr size_t tempsize = 1024;
		uint8_t temp[tempsize];
		size_t spos = 0;

		outlen /= sizeof(short);
		int done = 0;
		while(done < outlen)
		{
			size_t got = decoder->read((char*)temp, tempsize);
			for(size_t i = 0;i < got;++i)
				samples[spos++] = (short)((temp[i]-128) * 256);
			if(got < tempsize)
				break;
			done += got;
		}
	}
	delete decoder;
	return samples;
}

DLL_EXPORT struct SoundDecoder* zmsx_create_decoder(const uint8_t* data, size_t size, bool isstatic)
{
	MusicIO::FileInterface* reader;
	if (isstatic) reader = new MusicIO::MemoryReader(data, (long)size);
	else reader = new MusicIO::VectorReader(data, size);
	auto res = SoundDecoder::zmsx_create_decoder(reader);
	if (!res) reader->close();
	return res;
}

DLL_EXPORT void zmsx_sounddecoder_get_info(struct SoundDecoder* decoder, int* samplerate, zmsx_ChannelConfig* chans, zmsx_SampleType* type)
{
	if (decoder) decoder->getInfo(samplerate, chans, type);
	else if (samplerate) *samplerate = 0;
}

DLL_EXPORT size_t zmsx_sounddecoder_read(struct SoundDecoder* decoder, void* buffer, size_t length)
{
	if (decoder) return decoder->read((char*)buffer, length);
	else return 0;
}

DLL_EXPORT void zmsx_sounddecoder_close(struct SoundDecoder* decoder)
{
	if (decoder) delete decoder;
}
