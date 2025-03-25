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
typedef enum ZMSXMidiDeviceClass {
	zmsx_devcls_midiport = 1,
	zmsx_devcls_synth,
	zmsx_devcls_sqsynth,
	zmsx_devcls_fmsynth,
	zmsx_devcls_mapper,
	zmsx_devcls_wavetable,
	zmsx_devcls_swsynth,
} ZMSXMidiDeviceClass;

typedef enum ZMSXMidiType {
	zmsx_midi_notmidi,
	zmsx_midi_midi,
	zmsx_midi_hmi,
	zmsx_midi_xmi,
	zmsx_midi_mus,
	zmsx_midi_mids,
} ZMSXMidiType;

typedef enum ZMSXMidiDevice {
	zmsx_mdev_default = -1,
	zmsx_mdev_standard = 0,
	zmsx_mdev_opl = 1,
	zmsx_mdev_sndsys = 2,
	zmsx_mdev_timidity = 3,
	zmsx_mdev_fluidsynth = 4,
	zmsx_mdev_gus = 5,
	zmsx_mdev_wildmidi = 6,
	zmsx_mdev_adl = 7,
	zmsx_mdev_opn = 8,

	_zmsx_mdev_count_,
} ZMSXMidiDevice;

typedef enum ZMSXSoundFontTypes {
	zmsx_sf_sf2 = 1,
	zmsx_sf_gus = 2,
	zmsx_sf_wopl = 4,
	zmsx_sf_wopn = 8,
} ZMSXSoundFontTypes;

typedef struct ZMSXSoundStreamInfo {
	/// If 0, the song doesn't use streaming
	/// but plays through a different interface.
	int buffer_size;
	int sample_rate;
	/// If negative, 16 bit integer format is used instead of floating point.
	int num_channels;
} ZMSXSoundStreamInfo;

typedef enum ZMSXSampleType {
	zmsx_sample_uint8,
	zmsx_sample_int16,
	zmsx_sample_float32
} ZMSXSampleType;

typedef enum ZMSXChannelConfig {
	zmsx_chancfg_mono,
	zmsx_chancfg_stereo
} ZMSXChannelConfig;

typedef struct ZMSXSoundStreamInfoEx {
	/// If 0, the song doesn't use streaming but plays through a different interface.
	int buffer_size;
	int sample_rate;
	ZMSXSampleType sample_type;
	ZMSXChannelConfig channel_config;
} ZMSXSoundStreamInfoEx;

typedef enum ZMSXIntConfigKey {
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
	zmsx_fluid_samplerate,
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
} ZMSXIntConfigKey;

typedef enum ZMSXFloatConfigKey {
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
} ZMSXFloatConfigKey;

typedef enum ZMSXStringConfigKey {
	zmsx_adl_custom_bank = 2000,
	zmsx_fluid_lib,
	zmsx_fluid_patchset,
	zmsx_opn_custom_bank,
	zmsx_gus_config,
	zmsx_gus_patchdir,
	zmsx_timidity_config,
	zmsx_wildmidi_config,

	NUM_STRING_CONFIGS
} ZMSXStringConfigKey;

typedef struct ZMSXCustomReader {
	void* handle;
	char* (*gets)(struct ZMSXCustomReader* handle, char* buff, int n);
	long (*read)(struct ZMSXCustomReader* handle, void* buff, int32_t size);
	long (*seek)(struct ZMSXCustomReader* handle, long offset, int whence);
	long (*tell)(struct ZMSXCustomReader* handle);
	void (*close)(struct ZMSXCustomReader* handle);
} ZMSXCustomReader;

typedef struct ZMSXMidiOutDevice {
	char *Name;
	int ID;
	int Technology;
} ZMSXMidiOutDevice;

typedef enum ZMSXMessageLevel {
	zmsx_msg_verbose = 1,
	zmsx_msg_debug = 5,
	zmsx_msg_notify = 10,
	zmsx_msg_warning = 50,
	zmsx_msg_error = 100,
	zmsx_msg_fatal = 666,
} ZMSXMessageLevel;

/// Callbacks the client can install to capture messages from the backends
/// or to provide sound font data.
typedef struct ZMSXCallbacks {
	/// Used to handle client-specific path macros (e.g. `~`, `.`, and `..`)
	/// and environment variables.
	/// If not set, the path may not contain any special tokens that may need expansion.
	const char *(*canonicalize)(const char* path);

	void (*message_func)(ZMSXMessageLevel severity, const char* msg);
	// The message callbacks are optional, without them the output goes to stdout.

	/// Retrieves the path to a soundfont identified by an identifier.
	/// Only needed if the client virtualizes the soundfont names.
	/// The returned path needs to be freed by the caller if applicable.
	const char *(*path_for_soundfont)(const char *name, int type);

	// The sound font callbacks are for allowing the client to customize sound font management and they are optional.
	// They only need to be defined if the client virtualizes the sound font management and doesn't pass real paths to the music code.
	// Without them only paths to real files can be used. If one of these gets set, all must be set.

	/// This opens a sound font. Must return a handle with which the sound font's content can be read.
	void *(*sf_open_soundfont)(const char* name, int type);

	/// Opens a file in the sound font. For GUS patch sets this will try to open each patch with this function.
	/// For other formats only the sound font's actual name can be requested.
	/// When passed NULL this must open the Timidity config file, if this is requested for an SF2 sound font it should be synthesized.
	ZMSXCustomReader* (*sf_open_file)(void* handle, const char* fn);

	/// Adds a path to the list of directories in which files must be looked for.
	void (*sf_add_to_search_path)(void* handle, const char* path);

	/// Closes the sound font reader.
	void (*sf_close)(void* handle);
} ZMSXCallbacks;

typedef enum ZMSXVarType {
	zmsx_var_int,
	zmsx_var_bool,
	zmsx_var_float,
	zmsx_var_string,
} ZMSXVarType;

typedef struct zmsx_Setting_ {
	const char* name;
	int identifier;
	ZMSXVarType type;
	float defaultVal;
	const char* defaultString;
} zmsx_Setting;

#ifndef ZMSX_HPP
#if defined(_MSC_VER) && !defined(ZMSX_STATIC)
#define DLL_IMPORT _declspec(dllimport)
#else // if defined(_MSC_VER) && !defined(ZMSX_STATIC)
#define DLL_IMPORT
#endif // if defined(_MSC_VER) && !defined(ZMSX_STATIC)

// Note that the internal 'class' definitions are not C compatible!
typedef struct ZMSXMidiSource { int zm1; } ZMSXMidiSource;
typedef struct ZMSXMusicStream { int zm2; } ZMSXMusicStream;
struct SoundDecoder;

#endif // ifndef ZMSX_HPP

#ifndef ZMUSIC_NO_PROTOTYPES

#ifdef __cplusplus
extern "C" {
#endif

	DLL_IMPORT const char* zmsx_get_last_error(void);

	/// Sets callbacks for functionality that the client needs to provide.
	DLL_IMPORT void zmsx_set_callbacks(const ZMSXCallbacks* callbacks);

	/// Sets GenMidi data for OPL playback.
	/// If this isn't provided the OPL synth will not work.
	DLL_IMPORT void zmsx_set_genmidi(const uint8_t* data);

	/// Set default bank for OPN. Without this OPN only works with custom banks.
	DLL_IMPORT void zmsx_set_wgopn(const void* data, unsigned len);

	/// Set DMXGUS data for running the GUS synth in actual GUS mode.
	DLL_IMPORT void zmsx_set_dmxgus(const void* data, unsigned len);

	/// Returns an array with all available configuration options,
	/// terminated with an empty entry where all elements are 0.
	DLL_IMPORT const zmsx_Setting* zmsx_get_config(void);

	/// These exports are needed by the MIDI dumpers which need to remain on the client side
	/// because they need access to the client's file system.
	DLL_IMPORT ZMSXMidiType zmsx_identify_midi_type(uint32_t* id, int size);

	DLL_IMPORT ZMSXMidiSource*
		zmsx_create_midi_source(const uint8_t* data, size_t length, ZMSXMidiType miditype);

	DLL_IMPORT bool zmsx_midi_dump_wave(
		ZMSXMidiSource* source,
		ZMSXMidiDevice devtype,
		const char* devarg,
		const char* outname,
		int subsong,
		int samplerate
	);

	DLL_IMPORT ZMSXMusicStream*
		zmsx_open_song(ZMSXCustomReader* reader, ZMSXMidiDevice device, const char* args);

	DLL_IMPORT ZMSXMusicStream*
		zmsx_open_song_file(const char* filename, ZMSXMidiDevice device, const char* args);

	DLL_IMPORT ZMSXMusicStream* zmsx_open_song_mem(
		const void* mem,
		size_t size,
		ZMSXMidiDevice device,
		const char* Args
	);

	DLL_IMPORT ZMSXMusicStream* zmsx_open_song_cd(int track, int cdid);

	DLL_IMPORT bool zmsx_fill_stream(ZMSXMusicStream* stream, void* buff, int len);

	DLL_IMPORT bool zmsx_start(ZMSXMusicStream* song, int subsong, bool loop);

	DLL_IMPORT void zmsx_pause(ZMSXMusicStream* song);

	DLL_IMPORT void zmsx_resume(ZMSXMusicStream* song);

	DLL_IMPORT void zmsx_update(ZMSXMusicStream* song);

	DLL_IMPORT bool zmsx_is_playing(ZMSXMusicStream* song);

	DLL_IMPORT void zmsx_stop(ZMSXMusicStream* song);

	DLL_IMPORT void zmsx_close(ZMSXMusicStream* song);

	DLL_IMPORT bool zmsx_set_subsong(ZMSXMusicStream* song, int subsong);

	DLL_IMPORT bool zmsx_is_looping(const ZMSXMusicStream* song);

	DLL_IMPORT int zmsx_get_device_type(const ZMSXMusicStream* song);

	DLL_IMPORT bool zmsx_is_midi(const ZMSXMusicStream* song);

	DLL_IMPORT void zmsx_volume_changed(ZMSXMusicStream* song);

	DLL_IMPORT bool
		zmsx_write_smf(ZMSXMidiSource source, const char* fn, int looplimit);

	DLL_IMPORT void zmsx_get_stream_info(ZMSXMusicStream* song, ZMSXSoundStreamInfo* info);

	DLL_IMPORT void zmsx_get_stream_info_ex(
		ZMSXMusicStream* song,
		ZMSXSoundStreamInfoEx* info
	);

	// Configuration interface. The return value specifies if a music restart is needed.
	// RealValue should be written back to the CVAR or whatever other method the client uses to store configuration state.

	DLL_IMPORT bool zmsx_config_set_int(
		ZMSXIntConfigKey key,
		ZMSXMusicStream* song,
		int value,
		int* pRealValue
	);

	DLL_IMPORT bool zmsx_config_set_float(
		ZMSXFloatConfigKey key,
		ZMSXMusicStream* song,
		float value,
		float* pRealValue
	);

	DLL_IMPORT bool zmsx_config_set_string(
		ZMSXStringConfigKey key,
		ZMSXMusicStream* song,
		const char* value
	);

	DLL_IMPORT const char* zmsx_get_stats(ZMSXMusicStream* song);

	DLL_IMPORT struct SoundDecoder* zmsx_create_decoder(
		const uint8_t* data,
		size_t size,
		bool isstatic
	);

	DLL_IMPORT void zmsx_sounddecoder_get_info(
		struct SoundDecoder* decoder,
		int* samplerate,
		ZMSXChannelConfig* chans,
		ZMSXSampleType* type
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

	DLL_IMPORT const ZMSXMidiOutDevice *zmsx_get_midi_devices(int *pAmount);

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
	ZMSXIntConfigKey key,
	ZMSXMusicStream* song,
	int value,
	int* pRealValue = nullptr
) {
	return zmsx_config_set_int(key, song, value, pRealValue);
}

inline bool ChangeMusicSetting(
	ZMSXFloatConfigKey key,
	ZMSXMusicStream* song,
	float value,
	float* pRealValue = nullptr
) {
	return zmsx_config_set_float(key, song, value, pRealValue);
}

inline bool ChangeMusicSetting(
	ZMSXStringConfigKey key,
	ZMSXMusicStream* song,
	const char* value
) {
	return zmsx_config_set_string(key, song, value);
}

#endif // ifdef __cplusplus
#endif // ifndef ZMUSIC_NO_PROTOTYPES

// Function typedefs for run-time linking.

typedef const char* (*pfn_zmsx_get_last_error)(void);

typedef void (*pfn_zmsx_set_callbacks)(const ZMSXCallbacks* callbacks);

typedef void (*pfn_zmsx_set_genmidi)(const uint8_t* data);

typedef void (*pfn_zmsx_set_wgopn)(const void* data, unsigned len);

typedef void (*pfn_zmsx_set_dmxgus)(const void* data, unsigned len);

typedef const zmsx_Setting* (*pfn_zmsx_get_config)();

typedef ZMSXMidiType (*pfn_zmsx_identify_midi_type)(uint32_t* id, int size);

typedef ZMSXMidiSource (*pfn_zmsx_create_midi_source)(
	const uint8_t* data,
	size_t length,
	ZMSXMidiType miditype
);

typedef bool (*pfn_zmsx_midi_dump_wave)(
	ZMSXMidiSource source,
	ZMSXMidiDevice devtype,
	const char* devarg,
	const char* outname,
	int subsong,
	int samplerate
);

typedef ZMSXMusicStream* (*pfn_zmsx_open_song)(
	ZMSXCustomReader* reader, ZMSXMidiDevice device, const char* Args
);

typedef ZMSXMusicStream* (*pfn_zmsx_open_song_file)(
	const char* filename, ZMSXMidiDevice device, const char* Args
);

typedef ZMSXMusicStream* (*pfn_zmsx_open_song_mem)(
	const void* mem, size_t size, ZMSXMidiDevice device, const char* Args
);

typedef ZMSXMusicStream* (*pfn_zmsx_open_song_cd)(int track, int cdid);

typedef bool (*pfn_zmsx_fill_stream)(
	ZMSXMusicStream* stream,
	void* buff,
	int len
);

typedef bool (*pfn_zmsx_start)(
	ZMSXMusicStream* song,
	int subsong,
	bool loop
);

typedef void (*pfn_zmsx_pause)(ZMSXMusicStream* song);

typedef void (*pfn_zmsx_resume)(ZMSXMusicStream* song);

typedef void (*pfn_zmsx_update)(ZMSXMusicStream* song);

typedef bool (*pfn_zmsx_is_playing)(ZMSXMusicStream* song);

typedef void (*pfn_zmsx_stop)(ZMSXMusicStream* song);

typedef void (*pfn_zmsx_close)(ZMSXMusicStream* song);

typedef bool (*pfn_zmsx_set_subsong)(ZMSXMusicStream* song, int subsong);

typedef bool (*pfn_zmsx_is_looping)(ZMSXMusicStream* song);

typedef bool (*pfn_zmsx_is_midi)(ZMSXMusicStream* song);

typedef void (*pfn_zmsx_volume_changed)(ZMSXMusicStream* song);

typedef bool (*pfn_zmsx_write_smf)(
	ZMSXMidiSource source,
	const char* fn,
	int looplimit
);

typedef void (*pfn_zmsx_get_stream_info)(ZMSXMusicStream* song, ZMSXSoundStreamInfo* info);

typedef void (*pfn_zmsx_get_stream_info_ex)(
	ZMSXMusicStream* song,
	ZMSXSoundStreamInfoEx* info
);

typedef bool (*pfn_zmsx_config_set_int)(
	ZMSXIntConfigKey key,
	ZMSXMusicStream* song,
	int value,
	int* pRealValue
);

typedef bool (*pfn_zmsx_config_set_float)(
	ZMSXFloatConfigKey key,
	ZMSXMusicStream* song,
	float value,
	float* pRealValue
);

typedef bool (*pfn_zmsx_config_set_string)(
	ZMSXStringConfigKey key,
	ZMSXMusicStream* song,
	const char* value
);

typedef const char* (*pfn_zmsx_get_stats)(ZMSXMusicStream* song);

typedef struct SoundDecoder* (*pfn_zmsx_create_decoder)(
	const uint8_t* data,
	size_t size,
	bool isstatic
);

typedef void (*pfn_zmsx_sounddecoder_get_info)(
	struct SoundDecoder* decoder,
	int* samplerate,
	ZMSXChannelConfig* chans,
	ZMSXSampleType* type
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

typedef const ZMSXMidiOutDevice* (*pfn_zmsx_get_midi_devices)(int* pAmount);

#endif
