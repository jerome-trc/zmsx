#ifndef ZMSX_H
#define ZMSX_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>

#if __STDC_VERSION__ >= 202000L || (defined(__cplusplus) && __cplusplus >= 201703L)
#define ZMSX_NODISCARD [[nodiscard]]
#else
#define ZMSX_NODISCARD
#endif

struct SoundDecoder;	// Anonymous to the client.

// These constants must match the corresponding values of the Windows headers
// to avoid readjustment in the native Windows device's playback functions
// and should not be changed.
typedef enum zmsx_MidiDeviceClass_ {
	zmsx_devcls_midiport = 1,
	zmsx_devcls_synth,
	zmsx_devcls_sqsynth,
	zmsx_devcls_fmsynth,
	zmsx_devcls_mapper,
	zmsx_devcls_wavetable,
	zmsx_devcls_swsynth
} zmsx_MidiDeviceClass;

typedef enum EMIDIType_ {
	MIDI_NOTMIDI,
	MIDI_MIDI,
	MIDI_HMI,
	MIDI_XMI,
	MIDI_MUS,
	MIDI_MIDS
} EMIDIType;

typedef enum EMidiDevice_ {
	MDEV_DEFAULT = -1,
	MDEV_STANDARD = 0,
	MDEV_OPL = 1,
	MDEV_SNDSYS = 2,
	MDEV_TIMIDITY = 3,
	MDEV_FLUIDSYNTH = 4,
	MDEV_GUS = 5,
	MDEV_WILDMIDI = 6,
	MDEV_ADL = 7,
	MDEV_OPN = 8,

	MDEV_COUNT
} EMidiDevice;

typedef enum ESoundFontTypes_ {
	SF_SF2 = 1,
	SF_GUS = 2,
	SF_WOPL = 4,
	SF_WOPN = 8
} ESoundFontTypes;

typedef struct SoundStreamInfo_ {
	/// If 0, the song doesn't use streaming
	/// but plays through a different interface.
	int mBufferSize;
	int mSampleRate;
	/// If negative, 16 bit integer format is used instead of floating point.
	int mNumChannels;
} SoundStreamInfo;

typedef enum SampleType_ {
	SampleType_UInt8,
	SampleType_Int16,
	SampleType_Float32
} SampleType;

typedef enum ChannelConfig_ {
	ChannelConfig_Mono,
	ChannelConfig_Stereo
} ChannelConfig;

typedef struct SoundStreamInfoEx_ {
	/// If 0, the song doesn't use streaming but plays through a different interface.
	int mBufferSize;
	int mSampleRate;
	SampleType mSampleType;
	ChannelConfig mChannelConfig;
} SoundStreamInfoEx;

typedef enum EIntConfigKey_ {
	zmusic_adl_chips_count,
	zmusic_adl_emulator_id,
	zmusic_adl_run_at_pcm_rate,
	zmusic_adl_fullpan,
	zmusic_adl_bank,
	zmusic_adl_use_custom_bank,
	zmusic_adl_volume_model,

	zmusic_fluid_reverb,
	zmusic_fluid_chorus,
	zmusic_fluid_voices,
	zmusic_fluid_interp,
	zmusic_fluid_samplerate,
	zmusic_fluid_threads,
	zmusic_fluid_chorus_voices,
	zmusic_fluid_chorus_type,

	zmusic_opl_numchips,
	zmusic_opl_core,
	zmusic_opl_fullpan,

	zmusic_opn_chips_count,
	zmusic_opn_emulator_id,
	zmusic_opn_run_at_pcm_rate,
	zmusic_opn_fullpan,
	zmusic_opn_use_custom_bank,

	zmusic_gus_dmxgus,
	zmusic_gus_midi_voices,
	zmusic_gus_memsize,

	zmusic_timidity_modulation_wheel,
	zmusic_timidity_portamento,
	zmusic_timidity_reverb,
	zmusic_timidity_reverb_level,
	zmusic_timidity_chorus,
	zmusic_timidity_surround_chorus,
	zmusic_timidity_channel_pressure,
	zmusic_timidity_lpf_def,
	zmusic_timidity_temper_control,
	zmusic_timidity_modulation_envelope,
	zmusic_timidity_overlap_voice_allow,
	zmusic_timidity_drum_effect,
	zmusic_timidity_pan_delay,
	zmusic_timidity_key_adjust,

	zmusic_wildmidi_reverb,
	zmusic_wildmidi_enhanced_resampling,

	zmusic_snd_midiprecache,

	zmusic_mod_samplerate,
	zmusic_mod_volramp,
	zmusic_mod_interp,
	zmusic_mod_autochip,
	zmusic_mod_autochip_size_force,
	zmusic_mod_autochip_size_scan,
	zmusic_mod_autochip_scan_threshold,

	zmusic_snd_streambuffersize,

	zmusic_snd_mididevice,
	zmusic_snd_outputrate,

	NUM_ZMUSIC_INT_CONFIGS
} EIntConfigKey;

typedef enum EFloatConfigKey_ {
	zmusic_fluid_gain = 1000,
	zmusic_fluid_reverb_roomsize,
	zmusic_fluid_reverb_damping,
	zmusic_fluid_reverb_width,
	zmusic_fluid_reverb_level,
	zmusic_fluid_chorus_level,
	zmusic_fluid_chorus_speed,
	zmusic_fluid_chorus_depth,

	zmusic_timidity_drum_power,
	zmusic_timidity_tempo_adjust,
	zmusic_timidity_min_sustain_time,

	zmusic_gme_stereodepth,
	zmusic_mod_dumb_mastervolume,

	zmusic_snd_musicvolume,
	zmusic_relative_volume,
	zmusic_snd_mastervolume,

	NUM_FLOAT_CONFIGS
} EFloatConfigKey;

typedef enum EStringConfigKey_ {
	zmusic_adl_custom_bank = 2000,
	zmusic_fluid_lib,
	zmusic_fluid_patchset,
	zmusic_opn_custom_bank,
	zmusic_gus_config,
	zmusic_gus_patchdir,
	zmusic_timidity_config,
	zmusic_wildmidi_config,

	NUM_STRING_CONFIGS
} EStringConfigKey;

typedef struct ZMusicCustomReader_ {
	void* handle;
	char* (*gets)(struct ZMusicCustomReader_* handle, char* buff, int n);
	long (*read)(struct ZMusicCustomReader_* handle, void* buff, int32_t size);
	long (*seek)(struct ZMusicCustomReader_* handle, long offset, int whence);
	long (*tell)(struct ZMusicCustomReader_* handle);
	void (*close)(struct ZMusicCustomReader_* handle);
} ZMusicCustomReader;

typedef struct ZMusicMidiOutDevice_ {
	char *Name;
	int ID;
	int Technology;
} ZMusicMidiOutDevice;

typedef enum EZMusicMessageSeverity_ {
	ZMUSIC_MSG_VERBOSE = 1,
	ZMUSIC_MSG_DEBUG = 5,
	ZMUSIC_MSG_NOTIFY = 10,
	ZMUSIC_MSG_WARNING = 50,
	ZMUSIC_MSG_ERROR = 100,
	ZMUSIC_MSG_FATAL = 666,
} EZMusicMessageSeverity;

typedef struct ZMusicCallbacks_ {
	/// Callbacks the client can install to capture messages from the backends
	/// or to provide sound font data.

	void (*MessageFunc)(int severity, const char* msg);
	// The message callbacks are optional, without them the output goes to stdout.

	/// Retrieves the path to a soundfont identified by an identifier. Only needed if the client virtualizes the sound font names
	const char *(*PathForSoundfont)(const char *name, int type);

	// The sound font callbacks are for allowing the client to customize sound font management and they are optional.
	// They only need to be defined if the client virtualizes the sound font management and doesn't pass real paths to the music code.
	// Without them only paths to real files can be used. If one of these gets set, all must be set.

	/// This opens a sound font. Must return a handle with which the sound font's content can be read.
	void *(*OpenSoundFont)(const char* name, int type);

	/// Opens a file in the sound font. For GUS patch sets this will try to open each patch with this function.
	/// For other formats only the sound font's actual name can be requested.
	/// When passed NULL this must open the Timidity config file, if this is requested for an SF2 sound font it should be synthesized.
	ZMusicCustomReader* (*SF_OpenFile)(void* handle, const char* fn);

	/// Adds a path to the list of directories in which files must be looked for.
	void (*SF_AddToSearchPath)(void* handle, const char* path);

	// Closes the sound font reader.
	void (*SF_Close)(void* handle);

	/// Used to handle client-specific path macros.
	/// If not set, the path may not contain any special tokens that may need expansion.
	const char *(*NicePath)(const char* path);
} ZMusicCallbacks;

typedef enum ZMusicVariableType_ {
	ZMUSIC_VAR_INT,
	ZMUSIC_VAR_BOOL,
	ZMUSIC_VAR_FLOAT,
	ZMUSIC_VAR_STRING,
} ZMusicVariableType;

typedef struct ZMSXConfigurationSetting_ {
	const char* name;
	int identifier;
	ZMusicVariableType type;
	float defaultVal;
	const char* defaultString;
} ZMSXConfigurationSetting;

#ifndef ZMSX_HPP
#if defined(_MSC_VER) && !defined(ZMSX_STATIC)
#define DLL_IMPORT _declspec(dllimport)
#else // if defined(_MSC_VER) && !defined(ZMSX_STATIC)
#define DLL_IMPORT
#endif // if defined(_MSC_VER) && !defined(ZMSX_STATIC)

// Note that the internal 'class' definitions are not C compatible!
typedef struct _ZMusic_MidiSource_Struct { int zm1; } *ZMusic_MidiSource;
typedef struct _ZMusic_MusicStream_Struct { int zm2; } *ZMusic_MusicStream;
struct SoundDecoder;

#endif // ifndef ZMSX_HPP

#ifndef ZMUSIC_NO_PROTOTYPES

#ifdef __cplusplus
extern "C" {
#endif

	DLL_IMPORT const char* zmsx_get_last_error(void);

	/// Sets callbacks for functionality that the client needs to provide.
	DLL_IMPORT void zmsx_set_callbacks(const ZMusicCallbacks* callbacks);

	/// Sets GenMidi data for OPL playback.
	/// If this isn't provided the OPL synth will not work.
	DLL_IMPORT void zmsx_set_genmidi(const uint8_t* data);

	/// Set default bank for OPN. Without this OPN only works with custom banks.
	DLL_IMPORT void zmsx_set_wgopn(const void* data, unsigned len);

	/// Set DMXGUS data for running the GUS synth in actual GUS mode.
	DLL_IMPORT void zmsx_set_dmxgus(const void* data, unsigned len);

	/// Returns an array with all available configuration options,
	/// terminated with an empty entry where all elements are 0.
	DLL_IMPORT const ZMSXConfigurationSetting* zmsx_get_config(void);

	/// These exports are needed by the MIDI dumpers which need to remain on the client side
	/// because they need access to the client's file system.
	DLL_IMPORT EMIDIType zmsx_identify_midi_type(uint32_t* id, int size);

	DLL_IMPORT ZMusic_MidiSource
		zmsx_create_midi_source(const uint8_t* data, size_t length, EMIDIType miditype);

	DLL_IMPORT bool zmsx_midi_dump_wave(
		ZMusic_MidiSource source,
		EMidiDevice devtype,
		const char* devarg,
		const char* outname,
		int subsong,
		int samplerate
	);

	DLL_IMPORT ZMusic_MusicStream
		zmsx_open_song(ZMusicCustomReader* reader, EMidiDevice device, const char* Args);

	DLL_IMPORT ZMusic_MusicStream
		zmsx_open_song_file(const char* filename, EMidiDevice device, const char* Args);

	DLL_IMPORT ZMusic_MusicStream zmsx_open_song_mem(
		const void* mem,
		size_t size,
		EMidiDevice device,
		const char* Args
	);

	DLL_IMPORT ZMusic_MusicStream zmsx_open_song_cd(int track, int cdid);

	DLL_IMPORT bool zmsx_fill_stream(ZMusic_MusicStream stream, void* buff, int len);

	DLL_IMPORT bool zmsx_start(ZMusic_MusicStream song, int subsong, bool loop);

	DLL_IMPORT void zmsx_pause(ZMusic_MusicStream song);

	DLL_IMPORT void zmsx_resume(ZMusic_MusicStream song);

	DLL_IMPORT void zmsx_update(ZMusic_MusicStream song);

	DLL_IMPORT bool zmsx_is_playing(ZMusic_MusicStream song);

	DLL_IMPORT void zmsx_stop(ZMusic_MusicStream song);

	DLL_IMPORT void zmsx_close(ZMusic_MusicStream song);

	DLL_IMPORT bool zmsx_set_subsong(ZMusic_MusicStream song, int subsong);

	DLL_IMPORT bool zmsx_is_looping(ZMusic_MusicStream song);

	DLL_IMPORT int zmsx_get_device_type(ZMusic_MusicStream song);

	DLL_IMPORT bool zmsx_is_midi(ZMusic_MusicStream song);

	DLL_IMPORT void zmsx_volume_changed(ZMusic_MusicStream song);

	DLL_IMPORT bool
		zmsx_write_smf(ZMusic_MidiSource source, const char* fn, int looplimit);

	DLL_IMPORT void zmsx_get_stream_info(ZMusic_MusicStream song, SoundStreamInfo* info);

	DLL_IMPORT void zmsx_get_stream_info_ex(
		ZMusic_MusicStream song,
		SoundStreamInfoEx* info
	);

	// Configuration interface. The return value specifies if a music restart is needed.
	// RealValue should be written back to the CVAR or whatever other method the client uses to store configuration state.

	DLL_IMPORT bool zmsx_config_set_int(
		EIntConfigKey key,
		ZMusic_MusicStream song,
		int value,
		int* pRealValue
	);

	DLL_IMPORT bool zmsx_config_set_float(
		EFloatConfigKey key,
		ZMusic_MusicStream song,
		float value,
		float* pRealValue
	);

	DLL_IMPORT bool zmsx_config_set_string(
		EStringConfigKey key,
		ZMusic_MusicStream song,
		const char* value
	);

	DLL_IMPORT const char* zmsx_get_stats(ZMusic_MusicStream song);

	DLL_IMPORT struct SoundDecoder* zmsx_create_decoder(
		const uint8_t* data,
		size_t size,
		bool isstatic
	);

	DLL_IMPORT void zmsx_sounddecoder_get_info(
		struct SoundDecoder* decoder,
		int* samplerate,
		ChannelConfig* chans,
		SampleType* type
	);

	DLL_IMPORT size_t
		zmsx_sounddecoder_read(struct SoundDecoder* decoder, void* buffer, size_t length);

	DLL_IMPORT void zmsx_sounddecoder_close(struct SoundDecoder* decoder);

	DLL_IMPORT void zmsx_find_loop_tags(
		const uint8_t* data,
		size_t size,
		uint32_t* start,
		bool* startass,
		uint32_t* end,
		bool* endass
	);

	// The rest of the decoder interface is only useful for streaming music.

	DLL_IMPORT const ZMusicMidiOutDevice *zmsx_get_midi_devices(int *pAmount);

	DLL_IMPORT int zmsx_get_adl_banks(const char* const** pNames);

	// Direct access to the CD drive.
	// Stops playing the CD
	DLL_IMPORT void zmsx_cd_stop(void);

	// Pauses CD playing
	DLL_IMPORT void zmsx_cd_pause(void);

	// Resumes CD playback after pausing
	DLL_IMPORT bool zmsx_cd_resume(void);

	// Eject the CD tray
	DLL_IMPORT void zmsx_cd_eject(void);

	// Close the CD tray
	DLL_IMPORT bool zmsx_cd_uneject(void);

	// Closes a CD device previously opened with CD_Init
	DLL_IMPORT void zmsx_cd_close(void);

	DLL_IMPORT bool zmsx_cd_enable(const char* drive);

#ifdef __cplusplus
}

inline bool ChangeMusicSetting(
	EIntConfigKey key,
	ZMusic_MusicStream song,
	int value,
	int* pRealValue = nullptr
) {
	return zmsx_config_set_int(key, song, value, pRealValue);
}

inline bool ChangeMusicSetting(
	EFloatConfigKey key,
	ZMusic_MusicStream song,
	float value,
	float* pRealValue = nullptr
) {
	return zmsx_config_set_float(key, song, value, pRealValue);
}

inline bool ChangeMusicSetting(
	EStringConfigKey key,
	ZMusic_MusicStream song,
	const char* value
) {
	return zmsx_config_set_string(key, song, value);
}

#endif // ifdef __cplusplus
#endif // ifndef ZMUSIC_NO_PROTOTYPES

// Function typedefs for run-time linking.

typedef const char* (*pfn_zmsx_get_last_error)(void);

typedef void (*pfn_zmsx_set_callbacks)(const ZMusicCallbacks* callbacks);

typedef void (*pfn_zmsx_set_genmidi)(const uint8_t* data);

typedef void (*pfn_zmsx_set_wgopn)(const void* data, unsigned len);

typedef void (*pfn_zmsx_set_dmxgus)(const void* data, unsigned len);

typedef const ZMSXConfigurationSetting* (*pfn_zmsx_get_config)();

typedef EMIDIType (*pfn_zmsx_identify_midi_type)(uint32_t* id, int size);

typedef ZMusic_MidiSource (*pfn_zmsx_create_midi_source)(
	const uint8_t* data,
	size_t length,
	EMIDIType miditype
);

typedef bool (*pfn_zmsx_midi_dump_wave)(
	ZMusic_MidiSource source,
	EMidiDevice devtype,
	const char* devarg,
	const char* outname,
	int subsong,
	int samplerate
);

typedef ZMusic_MusicStream (*pfn_zmsx_open_song)(
	ZMusicCustomReader* reader, EMidiDevice device, const char* Args
);

typedef ZMusic_MusicStream (*pfn_zmsx_open_song_file)(
	const char* filename, EMidiDevice device, const char* Args
);

typedef ZMusic_MusicStream (*pfn_zmsx_open_song_mem)(
	const void* mem, size_t size, EMidiDevice device, const char* Args
);

typedef ZMusic_MusicStream (*pfn_zmsx_open_song_cd)(int track, int cdid);

typedef bool (*pfn_zmsx_fill_stream)(
	ZMusic_MusicStream stream,
	void* buff,
	int len
);

typedef bool (*pfn_zmsx_start)(
	ZMusic_MusicStream song,
	int subsong,
	bool loop
);

typedef void (*pfn_zmsx_pause)(ZMusic_MusicStream song);

typedef void (*pfn_zmsx_resume)(ZMusic_MusicStream song);

typedef void (*pfn_zmsx_update)(ZMusic_MusicStream song);

typedef bool (*pfn_zmsx_is_playing)(ZMusic_MusicStream song);

typedef void (*pfn_zmsx_stop)(ZMusic_MusicStream song);

typedef void (*pfn_zmsx_close)(ZMusic_MusicStream song);

typedef bool (*pfn_zmsx_set_subsong)(ZMusic_MusicStream song, int subsong);

typedef bool (*pfn_zmsx_is_looping)(ZMusic_MusicStream song);

typedef bool (*pfn_zmsx_is_midi)(ZMusic_MusicStream song);

typedef void (*pfn_zmsx_volume_changed)(ZMusic_MusicStream song);

typedef bool (*pfn_zmsx_write_smf)(
	ZMusic_MidiSource source,
	const char* fn,
	int looplimit
);

typedef void (*pfn_zmsx_get_stream_info)(ZMusic_MusicStream song, SoundStreamInfo* info);

typedef void (*pfn_zmsx_get_stream_info_ex)(
	ZMusic_MusicStream song,
	SoundStreamInfoEx* info
);

typedef bool (*pfn_zmsx_config_set_int)(
	EIntConfigKey key,
	ZMusic_MusicStream song,
	int value,
	int* pRealValue
);

typedef bool (*pfn_zmsx_config_set_float)(
	EFloatConfigKey key,
	ZMusic_MusicStream song,
	float value,
	float* pRealValue
);

typedef bool (*pfn_zmsx_config_set_string)(
	EStringConfigKey key,
	ZMusic_MusicStream song,
	const char* value
);

typedef const char* (*pfn_zmsx_get_stats)(ZMusic_MusicStream song);

typedef struct SoundDecoder* (*pfn_zmsx_create_decoder)(
	const uint8_t* data,
	size_t size,
	bool isstatic
);

typedef void (*pfn_zmsx_sounddecoder_get_info)(
	struct SoundDecoder* decoder,
	int* samplerate,
	ChannelConfig* chans,
	SampleType* type
);

typedef size_t (*pfn_zmsx_sounddecoder_read)(
	struct SoundDecoder* decoder,
	void* buffer,
	size_t length
);

typedef void (*pfn_zmsx_sounddecoder_close)(struct SoundDecoder* decoder);

typedef void (*pfn_zmsx_find_loop_tags)(
	const uint8_t* data,
	size_t size,
	uint32_t* start,
	bool* startass,
	uint32_t* end,
	bool* endass
);

typedef const ZMusicMidiOutDevice* (*pfn_zmsx_get_midi_devices)(int* pAmount);

#endif
