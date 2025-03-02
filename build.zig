const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const tc = b.addTranslateC(.{
        .optimize = optimize,
        .target = target,
        .root_source_file = b.path("include/zmsx.h"),
    });

    const lib = b.addLibrary(.{
        .linkage = .static,
        .name = "zmsx",
        .root_module = tc.addModule("zmsx.h"),
    });
    lib.linkLibC();
    lib.linkLibCpp();

    const common_flags = fast_math ++ stricmp ++ strnicmp ++ [_][]const u8{"-DHAVE_SNDFILE"};
    const c_flags = common_flags ++ [_][]const u8{"--std=c11"};
    const cxx_flags = common_flags ++ [_][]const u8{"--std=c++17"};

    lib.addCSourceFiles(.{
        .root = b.path("."),
        .flags = cxx_flags[0..],
        .files = &.{
            "source/loader/i_module.cpp",
            "source/mididevices/music_base_mididevice.cpp",
            "source/mididevices/music_adlmidi_mididevice.cpp",
            "source/mididevices/music_opl_mididevice.cpp",
            "source/mididevices/music_opnmidi_mididevice.cpp",
            "source/mididevices/music_timiditypp_mididevice.cpp",
            "source/mididevices/music_fluidsynth_mididevice.cpp",
            "source/mididevices/music_softsynth_mididevice.cpp",
            "source/mididevices/music_timidity_mididevice.cpp",
            "source/mididevices/music_wildmidi_mididevice.cpp",
            "source/mididevices/music_wavewriter_mididevice.cpp",
            "source/midisources/midisource.cpp",
            "source/midisources/midisource_mus.cpp",
            "source/midisources/midisource_smf.cpp",
            "source/midisources/midisource_hmi.cpp",
            "source/midisources/midisource_xmi.cpp",
            "source/midisources/midisource_mids.cpp",
            "source/streamsources/music_dumb.cpp",
            "source/streamsources/music_gme.cpp",
            "source/streamsources/music_libsndfile.cpp",
            "source/streamsources/music_opl.cpp",
            "source/streamsources/music_xa.cpp",
            "source/musicformats/music_stream.cpp",
            "source/musicformats/music_midi.cpp",
            "source/musicformats/music_cd.cpp",
            "source/decoder/sounddecoder.cpp",
            "source/decoder/sndfile_decoder.cpp",
            "source/decoder/mpg123_decoder.cpp",
            "source/zmsx/configuration.cpp",
            "source/zmsx/zmsx.cpp",
            "source/zmsx/critsec.cpp",
        },
    });
    lib.addCSourceFile(.{
        .file = b.path("source/loader/test.c"),
        .flags = c_flags[0..],
    });

    lib.addIncludePath(b.path("include"));
    lib.addIncludePath(b.path("source"));
    lib.addIncludePath(b.path("source/zmsx"));
    lib.addIncludePath(b.path("thirdparty/adlmidi"));
    lib.addIncludePath(b.path("thirdparty/dumb"));
    lib.addIncludePath(b.path("thirdparty/game-music-emu"));
    lib.addIncludePath(b.path("thirdparty/miniz"));
    lib.addIncludePath(b.path("thirdparty/oplsynth"));
    lib.addIncludePath(b.path("thirdparty/opnmidi"));
    lib.addIncludePath(b.path("thirdparty/timidity"));
    lib.addIncludePath(b.path("thirdparty/timidityplus"));
    lib.addIncludePath(b.path("thirdparty/wildmidi"));

    lib.addObject(adlmidi(b, optimize, target));
    lib.addObject(dumb(b, optimize, target));
    lib.addObject(fluidsynth(b, optimize, target));
    lib.addObject(gme(b, optimize, target));
    lib.addObject(miniz(b, optimize, target));
    lib.addObject(opl(b, optimize, target));
    lib.addObject(opn(b, optimize, target));
    lib.addObject(timidity(b, optimize, target));
    lib.addObject(timiditypp(b, optimize, target));
    lib.addObject(wildmidi(b, optimize, target));

    // Retrieve include paths via pkg-config.
    lib.linkSystemLibrary2("flac", .{
        .needed = true,
        .preferred_link_mode = .static,
        .use_pkg_config = .yes,
    });
    lib.linkSystemLibrary2("sndfile", .{
        .needed = true,
        .preferred_link_mode = .static,
        .use_pkg_config = .yes,
    });
    lib.linkSystemLibrary2("ogg", .{
        .needed = true,
        .preferred_link_mode = .static,
        .use_pkg_config = .yes,
    });
    lib.linkSystemLibrary2("opus", .{
        .needed = true,
        .preferred_link_mode = .static,
        .use_pkg_config = .yes,
    });
    lib.linkSystemLibrary2("vorbis", .{
        .needed = true,
        .preferred_link_mode = .static,
        .use_pkg_config = .yes,
    });
    lib.linkSystemLibrary2("vorbisenc", .{
        .needed = true,
        .preferred_link_mode = .static,
        .use_pkg_config = .yes,
    });

    b.installArtifact(lib);
}

fn adlmidi(
    b: *std.Build,
    optimize: std.builtin.OptimizeMode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step.Compile {
    const obj = b.addObject(.{
        .name = "adlmidi",
        .optimize = optimize,
        .target = target,
    });
    obj.linkLibC();
    obj.linkLibCpp();

    obj.addCSourceFiles(.{
        .root = b.path("thirdparty/adlmidi"),
        .flags = &(fast_math ++ [_][]const u8{
            "--std=c11",
            "-DADLMIDI_DISABLE_MIDI_SEQUENCER",
        }),
        .files = &[_][]const u8{
            "chips/nuked/nukedopl3_174.c",
            "chips/nuked/nukedopl3.c",
            "wopl/wopl_file.c",
        },
    });

    obj.addCSourceFiles(.{
        .root = b.path("thirdparty/adlmidi"),
        .flags = &(fast_math ++ [_][]const u8{
            "--std=c++17",
            "-DADLMIDI_DISABLE_MIDI_SEQUENCER",
        }),
        .files = &[_][]const u8{
            "adlmidi_midiplay.cpp",
            "adlmidi_opl3.cpp",
            "adlmidi_private.cpp",
            "adlmidi.cpp",
            "adlmidi_load.cpp",
            "inst_db.cpp",
            "chips/opal_opl3.cpp",
            "chips/dosbox/dbopl.cpp",
            "chips/nuked_opl3_v174.cpp",
            "chips/java_opl3.cpp",
            "chips/dosbox_opl3.cpp",
            "chips/nuked_opl3.cpp",
        },
    });

    obj.addIncludePath(b.path("thirdparty/adlmidi"));

    return obj;
}

fn dumb(
    b: *std.Build,
    optimize: std.builtin.OptimizeMode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step.Compile {
    const obj = b.addObject(.{
        .name = "dumb",
        .optimize = optimize,
        .target = target,
    });
    obj.linkLibC();

    obj.addCSourceFiles(.{
        .root = b.path("thirdparty/dumb"),
        .flags = &(fast_math ++ [_][]const u8{
            // Must be compiled with ANSI C
            "-DNEED_ITOA=1",
            // TODO: "-msse",
        }),
        .files = &[_][]const u8{
            "src/core/unload.c",
            "src/core/rendsig.c",
            "src/core/rendduh.c",
            "src/core/register.c",
            "src/core/readduh.c",
            "src/core/rawsig.c",
            "src/core/makeduh.c",
            "src/core/loadduh.c",
            "src/core/dumbfile.c",
            "src/core/duhtag.c",
            "src/core/duhlen.c",
            "src/core/atexit.c",
            "src/helpers/stdfile.c",
            "src/helpers/silence.c",
            "src/helpers/sampbuf.c",
            "src/helpers/riff.c",
            "src/helpers/resample.c",
            "src/helpers/memfile.c",
            "src/helpers/clickrem.c",
            "src/helpers/barray.c",
            "src/it/xmeffect.c",
            "src/it/readxm2.c",
            "src/it/readxm.c",
            "src/it/readstm2.c",
            "src/it/readstm.c",
            "src/it/reads3m2.c",
            "src/it/reads3m.c",
            "src/it/readriff.c",
            "src/it/readptm.c",
            "src/it/readpsm.c",
            "src/it/readoldpsm.c",
            "src/it/readokt2.c",
            "src/it/readokt.c",
            "src/it/readmtm.c",
            "src/it/readmod2.c",
            "src/it/readmod.c",
            "src/it/readdsmf.c",
            "src/it/readasy.c",
            "src/it/readamf2.c",
            "src/it/readamf.c",
            "src/it/readam.c",
            "src/it/read6692.c",
            "src/it/read669.c",
            "src/it/ptmeffect.c",
            "src/it/loadxm2.c",
            "src/it/loadxm.c",
            "src/it/loadstm2.c",
            "src/it/loadstm.c",
            "src/it/loads3m2.c",
            "src/it/loads3m.c",
            "src/it/loadriff2.c",
            "src/it/loadriff.c",
            "src/it/loadptm2.c",
            "src/it/loadptm.c",
            "src/it/loadpsm2.c",
            "src/it/loadpsm.c",
            "src/it/loadoldpsm2.c",
            "src/it/loadoldpsm.c",
            "src/it/loadokt2.c",
            "src/it/loadokt.c",
            "src/it/loadmtm2.c",
            "src/it/loadmtm.c",
            "src/it/loadmod2.c",
            "src/it/loadmod.c",
            "src/it/loadasy2.c",
            "src/it/loadasy.c",
            "src/it/loadamf2.c",
            "src/it/loadamf.c",
            "src/it/load6692.c",
            "src/it/load669.c",
            "src/it/itunload.c",
            "src/it/itrender.c",
            "src/it/itread2.c",
            "src/it/itread.c",
            "src/it/itorder.c",
            "src/it/itmisc.c",
            "src/it/itload2.c",
            "src/it/itload.c",
            "src/it/readany.c",
            "src/it/loadany2.c",
            "src/it/loadany.c",
            "src/it/readany2.c",
            "src/helpers/resampler.c",
            "src/helpers/lpc.c",
        },
    });

    obj.addIncludePath(b.path("thirdparty/dumb/include"));

    return obj;
}

fn fluidsynth(
    b: *std.Build,
    optimize: std.builtin.OptimizeMode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step.Compile {
    const obj = b.addObject(.{
        .name = "fluidsynth",
        .optimize = optimize,
        .target = target,
    });
    obj.linkLibCpp();

    var fluidsynth_flags = std.ArrayListUnmanaged([]const u8).initCapacity(b.allocator, 1) catch
        @panic("OOM");

    if (target.result.os.tag != .windows) {
        const flags = b.run(&[_][]const u8{ "pkg-config", "--cflags", "glib-2.0" });
        var iter = std.mem.splitScalar(u8, flags, ' ');

        while (iter.next()) |flag| {
            const f = std.mem.trim(u8, flag, " \n\r\t");
            fluidsynth_flags.append(b.allocator, f) catch @panic("OOM");
        }
    }

    obj.addCSourceFiles(.{
        .root = b.path("."),
        .flags = fluidsynth_flags.items,
        .files = &[_][]const u8{
            "thirdparty/fluidsynth/src/utils/fluid_conv.c",
            "thirdparty/fluidsynth/src/utils/fluid_hash.c",
            "thirdparty/fluidsynth/src/utils/fluid_list.c",
            "thirdparty/fluidsynth/src/utils/fluid_ringbuffer.c",
            "thirdparty/fluidsynth/src/utils/fluid_settings.c",
            "thirdparty/fluidsynth/src/utils/fluid_sys.c",
            "thirdparty/fluidsynth/src/sfloader/fluid_defsfont.c",
            "thirdparty/fluidsynth/src/sfloader/fluid_sfont.c",
            "thirdparty/fluidsynth/src/sfloader/fluid_sffile.c",
            "thirdparty/fluidsynth/src/sfloader/fluid_samplecache.c",
            "thirdparty/fluidsynth/src/rvoice/fluid_adsr_env.c",
            "thirdparty/fluidsynth/src/rvoice/fluid_chorus.c",
            "thirdparty/fluidsynth/src/rvoice/fluid_iir_filter.c",
            "thirdparty/fluidsynth/src/rvoice/fluid_lfo.c",
            "thirdparty/fluidsynth/src/rvoice/fluid_rvoice.c",
            "thirdparty/fluidsynth/src/rvoice/fluid_rvoice_dsp.c",
            "thirdparty/fluidsynth/src/rvoice/fluid_rvoice_event.c",
            "thirdparty/fluidsynth/src/rvoice/fluid_rvoice_mixer.c",
            "thirdparty/fluidsynth/src/rvoice/fluid_rev.c",
            "thirdparty/fluidsynth/src/synth/fluid_chan.c",
            "thirdparty/fluidsynth/src/synth/fluid_event.c",
            "thirdparty/fluidsynth/src/synth/fluid_gen.c",
            "thirdparty/fluidsynth/src/synth/fluid_mod.c",
            "thirdparty/fluidsynth/src/synth/fluid_synth.c",
            "thirdparty/fluidsynth/src/synth/fluid_synth_monopoly.c",
            "thirdparty/fluidsynth/src/synth/fluid_tuning.c",
            "thirdparty/fluidsynth/src/synth/fluid_voice.c",
            "thirdparty/fluidsynth/src/midi/fluid_midi.c",
            "thirdparty/fluidsynth/src/midi/fluid_midi_router.c",
            "thirdparty/fluidsynth/src/midi/fluid_seqbind.c",
            "thirdparty/fluidsynth/src/midi/fluid_seqbind_notes.cpp",
            "thirdparty/fluidsynth/src/midi/fluid_seq.c",
            "thirdparty/fluidsynth/src/midi/fluid_seq_queue.cpp",
            "thirdparty/fluidsynth/src/drivers/fluid_adriver.c",
            "thirdparty/fluidsynth/src/drivers/fluid_mdriver.c",
            "thirdparty/fluidsynth/src/bindings/fluid_filerenderer.c",
            "thirdparty/fluidsynth/src/bindings/fluid_ladspa.c",
        },
    });

    if (target.result.os.tag != .windows) {
        obj.linkSystemLibrary2("glib-2.0", .{
            .needed = true,
            .preferred_link_mode = .static,
            .use_pkg_config = .yes,
        });
    }

    obj.addIncludePath(b.path("source/decoder"));
    obj.addIncludePath(b.path("thirdparty"));
    obj.addIncludePath(b.path("thirdparty/fluidsynth/include"));
    obj.addIncludePath(b.path("thirdparty/fluidsynth/src"));
    obj.addIncludePath(b.path("thirdparty/fluidsynth/src/bindings"));
    obj.addIncludePath(b.path("thirdparty/fluidsynth/src/drivers"));
    obj.addIncludePath(b.path("thirdparty/fluidsynth/src/midi"));
    obj.addIncludePath(b.path("thirdparty/fluidsynth/src/rvoice"));
    obj.addIncludePath(b.path("thirdparty/fluidsynth/src/sfloader"));
    obj.addIncludePath(b.path("thirdparty/fluidsynth/src/synth"));
    obj.addIncludePath(b.path("thirdparty/fluidsynth/src/utils"));

    return obj;
}

fn gme(
    b: *std.Build,
    optimize: std.builtin.OptimizeMode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step.Compile {
    const obj = b.addObject(.{
        .name = "gme",
        .optimize = optimize,
        .target = target,
    });
    obj.linkLibC();
    obj.linkLibCpp();

    obj.addCSourceFiles(.{
        .root = b.path("thirdparty"),
        .flags = &.{
            "--std=c++17",
            "-fomit-frame-pointer",
            "-fwrapv",
            "-fno-sanitize=undefined",
            "-DHAVE_ZLIB_H",
            "-DUSE_GME_AY=1",
            "-DUSE_GME_GBS=1",
            "-DUSE_GME_GYM=1",
            "-DUSE_GME_HES=1",
            "-DUSE_GME_KSS=1",
            "-DUSE_GME_NSF=1",
            "-DUSE_GME_NSFE=1",
            "-DUSE_GME_SAP=1",
            "-DUSE_GME_SPC=1",
            "-DUSE_GME_VGM=1",
            "-DVGM_YM2612_NUKED",
            "-DUSE_GME_NSF=1",
        },
        .files = &.{
            "game-music-emu/gme/Blip_Buffer.cpp",
            "game-music-emu/gme/Classic_Emu.cpp",
            "game-music-emu/gme/Data_Reader.cpp",
            "game-music-emu/gme/Dual_Resampler.cpp",
            "game-music-emu/gme/Effects_Buffer.cpp",
            "game-music-emu/gme/Fir_Resampler.cpp",
            "game-music-emu/gme/gme.cpp",
            "game-music-emu/gme/Gme_File.cpp",
            "game-music-emu/gme/M3u_Playlist.cpp",
            "game-music-emu/gme/Multi_Buffer.cpp",
            "game-music-emu/gme/Music_Emu.cpp",
            // USE_GME_AY or USE_GME_KSS
            "game-music-emu/gme/Ay_Apu.cpp",
            // USE_GME_AY
            "game-music-emu/gme/Ay_Cpu.cpp",
            "game-music-emu/gme/Ay_Emu.cpp",
            // USE_GME_GBS
            "game-music-emu/gme/Gb_Apu.cpp",
            "game-music-emu/gme/Gb_Cpu.cpp",
            "game-music-emu/gme/Gb_Oscs.cpp",
            "game-music-emu/gme/Gbs_Emu.cpp",
            // USE_GME_GYM
            "game-music-emu/gme/Gym_Emu.cpp",
            // USE_GME_HES
            "game-music-emu/gme/Hes_Apu.cpp",
            "game-music-emu/gme/Hes_Cpu.cpp",
            "game-music-emu/gme/Hes_Emu.cpp",
            // USE_GME_KSS
            "game-music-emu/gme/Kss_Cpu.cpp",
            "game-music-emu/gme/Kss_Emu.cpp",
            "game-music-emu/gme/Kss_Scc_Apu.cpp",
            // USE_GME_NSF or USE_GME_NSFE
            "game-music-emu/gme/Nes_Apu.cpp",
            "game-music-emu/gme/Nes_Cpu.cpp",
            "game-music-emu/gme/Nes_Fme7_Apu.cpp",
            "game-music-emu/gme/Nes_Namco_Apu.cpp",
            "game-music-emu/gme/Nes_Oscs.cpp",
            "game-music-emu/gme/Nes_Vrc6_Apu.cpp",
            "game-music-emu/gme/Nsf_Emu.cpp",
            // USE_GME_NSFE
            "game-music-emu/gme/Nsfe_Emu.cpp",
            // USE_GME_SAP
            "game-music-emu/gme/Sap_Apu.cpp",
            "game-music-emu/gme/Sap_Cpu.cpp",
            "game-music-emu/gme/Sap_Emu.cpp",
            // USE_GME_SPC
            "game-music-emu/gme/Snes_Spc.cpp",
            "game-music-emu/gme/Spc_Cpu.cpp",
            "game-music-emu/gme/Spc_Dsp.cpp",
            "game-music-emu/gme/Spc_Emu.cpp",
            "game-music-emu/gme/Spc_Filter.cpp",
            // USE_GME_VGM
            "game-music-emu/gme/Vgm_Emu.cpp",
            "game-music-emu/gme/Vgm_Emu_Impl.cpp",
            "game-music-emu/gme/Ym2413_Emu.cpp",
            // USE_GME_VGM or USE_GME_GYM or USE_GME_KSS
            "game-music-emu/gme/Sms_Apu.cpp",
            // (USE_GME_VGM or USE_GME_GYM) and GME_YM2612_EMU == "Nuked"
            "game-music-emu/gme/Ym2612_Nuked.cpp",
        },
    });

    obj.addIncludePath(b.path("game-music-emu/gme"));
    obj.addIncludePath(b.path("thirdparty/miniz"));

    return obj;
}

fn miniz(
    b: *std.Build,
    optimize: std.builtin.OptimizeMode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step.Compile {
    const obj = b.addObject(.{
        .name = "miniz",
        .optimize = optimize,
        .target = target,
    });
    obj.linkLibC();

    obj.addCSourceFile(.{
        .file = b.path("thirdparty/miniz/miniz.c"),
        .flags = &.{ "--std=c11", "-DMINIZ_NO_STDIO" },
    });

    obj.addIncludePath(b.path("thirdparty/miniz"));

    return obj;
}

fn opl(
    b: *std.Build,
    optimize: std.builtin.OptimizeMode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step.Compile {
    const obj = b.addObject(.{
        .name = "oplsynth",
        .optimize = optimize,
        .target = target,
    });
    obj.linkLibC();
    obj.linkLibCpp();

    obj.addCSourceFiles(.{
        .root = b.path("thirdparty/oplsynth"),
        .flags = &(fast_math ++ stricmp ++ strnicmp ++ [_][]const u8{
            "--std=c++17",
            "-fomit-frame-pointer",
        }),
        .files = &[_][]const u8{
            "fmopl.cpp",
            "musicblock.cpp",
            "nukedopl3.cpp",
            "opl_mus_player.cpp",
            "OPL3.cpp",
            "oplio.cpp",
            "dosbox/opl.cpp",
        },
    });

    obj.addIncludePath(b.path("thirdparty/oplsynth"));
    obj.addIncludePath(b.path("thirdparty/oplsynth/oplsynth"));

    return obj;
}

fn opn(
    b: *std.Build,
    optimize: std.builtin.OptimizeMode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step.Compile {
    const obj = b.addObject(.{
        .name = "opnmidi",
        .optimize = optimize,
        .target = target,
    });
    obj.linkLibC();
    obj.linkLibCpp();

    obj.addCSourceFiles(.{
        .root = b.path("thirdparty/opnmidi"),
        .flags = &(fast_math ++ .{
            "--std=c11",
            "-DOPNMIDI_DISABLE_MIDI_SEQUENCER",
            "-DOPNMIDI_DISABLE_GX_EMULATOR",
        }),
        .files = &.{
            "chips/nuked/ym3438.c",
            "chips/gx/gx_ym2612.c",
            "chips/pmdwin/opna.c",
            "chips/pmdwin/psg.c",
            "chips/pmdwin/rhythmdata.c",
            "chips/mamefm/emu2149.c",
            "chips/mame/mame_ym2612fm.c",
            "wopn/wopn_file.c",
        },
    });

    obj.addCSourceFiles(.{
        .root = b.path("thirdparty/opnmidi"),
        .flags = &(fast_math ++ .{
            "--std=c++17",
            "-DOPNMIDI_DISABLE_MIDI_SEQUENCER",
            "-DOPNMIDI_DISABLE_GX_EMULATOR",
        }),
        .files = &.{
            "opnmidi_load.cpp",
            "opnmidi_private.cpp",
            "opnmidi.cpp",
            "opnmidi_midiplay.cpp",
            "opnmidi_opn2.cpp",
            "chips/np2/fmgen_fmgen.cpp",
            "chips/np2/fmgen_opna.cpp",
            "chips/np2/fmgen_fmtimer.cpp",
            "chips/np2/fmgen_file.cpp",
            "chips/np2/fmgen_psg.cpp",
            "chips/mame_opn2.cpp",
            "chips/gens_opn2.cpp",
            "chips/mame_opna.cpp",
            "chips/np2_opna.cpp",
            "chips/mamefm/ymdeltat.cpp",
            "chips/mamefm/resampler.cpp",
            "chips/mamefm/fm.cpp",
            "chips/nuked_opn2.cpp",
            "chips/gens/Ym2612.cpp",
            "chips/gx_opn2.cpp",
            "chips/pmdwin_opna.cpp",
        },
    });

    obj.addIncludePath(b.path("thirdparty/opnmidi"));

    return obj;
}

fn timidity(
    b: *std.Build,
    optimize: std.builtin.OptimizeMode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step.Compile {
    const obj = b.addObject(.{
        .name = "timidity",
        .target = target,
        .optimize = optimize,
    });
    obj.linkLibC();
    obj.linkLibCpp();

    obj.addCSourceFiles(.{
        .root = b.path("thirdparty/timidity"),
        .flags = &(fast_math ++ stricmp ++ .{"--std=c++17"}),
        .files = &.{
            "common.cpp",
            "instrum.cpp",
            "instrum_dls.cpp",
            "instrum_font.cpp",
            "instrum_sf2.cpp",
            "mix.cpp",
            "playmidi.cpp",
            "resample.cpp",
            "timidity.cpp",
        },
    });

    obj.addIncludePath(b.path("thirdparty/timidity"));
    obj.addIncludePath(b.path("thirdparty/timidity/timidity"));

    return obj;
}

fn timiditypp(
    b: *std.Build,
    optimize: std.builtin.OptimizeMode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step.Compile {
    const obj = b.addObject(.{
        .name = "timditypp",
        .optimize = optimize,
        .target = target,
    });
    obj.linkLibC();
    obj.linkLibCpp();

    obj.addCSourceFiles(.{
        .root = b.path("thirdparty/timidityplus"),
        .flags = &(fast_math ++ .{"--std=c++17"}),
        .files = &.{
            "fft4g.cpp",
            "reverb.cpp",
            "common.cpp",
            "configfile.cpp",
            "effect.cpp",
            "filter.cpp",
            "freq.cpp",
            "instrum.cpp",
            "mblock.cpp",
            "mix.cpp",
            "playmidi.cpp",
            "quantity.cpp",
            "readmidic.cpp",
            "recache.cpp",
            "resample.cpp",
            "sbkconv.cpp",
            "sffile.cpp",
            "sfitem.cpp",
            "smplfile.cpp",
            "sndfont.cpp",
            "tables.cpp",
        },
    });

    obj.addIncludePath(b.path("thirdparty/timidityplus"));
    obj.addIncludePath(b.path("thirdparty/timidityplus/timiditypp"));

    return obj;
}

fn wildmidi(
    b: *std.Build,
    optimize: std.builtin.OptimizeMode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step.Compile {
    const obj = b.addObject(.{
        .name = "wildmidi",
        .optimize = optimize,
        .target = target,
    });
    obj.linkLibC();
    obj.linkLibCpp();

    obj.addCSourceFiles(.{
        .root = b.path("thirdparty/wildmidi"),
        .flags = &(fast_math ++ stricmp ++ strnicmp ++ [_][]const u8{
            "--std=c++17",
            "-fomit-frame-pointer",
        }),
        .files = &[_][]const u8{
            "file_io.cpp",
            "gus_pat.cpp",
            "reverb.cpp",
            "wildmidi_lib.cpp",
            "wm_error.cpp",
        },
    });

    obj.addIncludePath(b.path("thirdparty/wildmidi"));
    obj.addIncludePath(b.path("thirdparty/wildmidi/wildmidi"));

    return obj;
}

const fast_math = [_][]const u8{ "-ffast-math", "-ffp-contract=fast" };
const stricmp = [_][]const u8{"-Dstricmp=strcasecmp"};
const strnicmp = [_][]const u8{"-Dstrnicmp=strncasecmp"};
