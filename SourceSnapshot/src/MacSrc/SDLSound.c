#include "Xmi.h"
#include "MusicDevice.h"

static snd_digi_parms digi_parms_by_channel[SND_MAX_SAMPLES];

#ifdef USE_SDL_MIXER

#include <SDL_mixer.h>
#include <SDL_log.h>

static Mix_Chunk *samples_by_channel[SND_MAX_SAMPLES];

extern SDL_AudioStream *cutscene_audiostream;
extern struct MusicDevice *MusicDev;

extern void AudioStreamCallback(void *userdata, unsigned char *stream, int len);
extern void MusicCallback(void *userdata, Uint8 *stream, int len);

#ifdef __ANDROID__
#define RP5NP_POSTMIX_SCRATCH_BYTES 8192
static Uint8 rp5np_postmix_scratch[RP5NP_POSTMIX_SCRATCH_BYTES];
static bool rp5np_first_sfx_logged = false;

/* Keep SDL_mixer ownership inside the sound layer. Game/cutscene code uses
   these narrow bridges instead of importing SDL_mixer headers. */
void rp5np_aux_audio_pause(void) { Mix_PauseAudio(1); }
void rp5np_aux_audio_resume(void) { Mix_PauseAudio(0); }

/* SDL_mixer owns the one physical Android audio device. Cutscene/audio-log
   streams are mixed into the already-composed game music + SFX buffer here. */
static void rp5np_external_stream_postmix(void *userdata, Uint8 *stream, int len) {
    SDL_AudioStream *active_stream = *(SDL_AudioStream **)userdata;
    if (active_stream == NULL || len <= 0)
        return;

    int offset = 0;
    while (offset < len) {
        int available = SDL_AudioStreamAvailable(active_stream);
        if (available <= 0)
            break;

        int wanted = len - offset;
        if (wanted > RP5NP_POSTMIX_SCRATCH_BYTES)
            wanted = RP5NP_POSTMIX_SCRATCH_BYTES;
        if (wanted > available)
            wanted = available;

        /* AUDIO_S16SYS stereo: never request a partial sample frame. */
        wanted &= ~3;
        if (wanted <= 0)
            break;

        int got = SDL_AudioStreamGet(active_stream, rp5np_postmix_scratch, wanted);
        if (got <= 0)
            break;
        got &= ~3;
        if (got <= 0)
            break;

        SDL_MixAudioFormat(stream + offset, rp5np_postmix_scratch, AUDIO_S16SYS, (Uint32)got, SDL_MIX_MAXVOLUME);
        offset += got;
    }
}
#endif

int snd_start_digital(void) {

    // Startup the sound system

#ifdef __ANDROID__
    /* One physical device only. The previous Android path opened a raw SDL
       device for cutscenes first, causing Mix_OpenAudio() to fail with
       "Audio device already open" and leaving in-game mixer channels silent. */
    int mix_init_flags = Mix_Init(MIX_INIT_MP3);
    if ((mix_init_flags & MIX_INIT_MP3) != MIX_INIT_MP3) {
        ERROR("%s: SDL_mixer MP3 init failed: %s", __FUNCTION__, Mix_GetError());
        SDL_Log("RP5NP_MIXER_INIT_FAIL flags=0x%x error=%s", mix_init_flags, Mix_GetError());
    } else {
        SDL_Log("RP5NP_MIXER_INIT_OK flags=0x%x", mix_init_flags);
    }

    /* The post-mix bridge consumes 48 kHz/S16/stereo data, so require that
       exact device mix format instead of allowing SDL_mixer to change it. */
    if (Mix_OpenAudioDevice(48000, AUDIO_S16SYS, 2, 1024, NULL, 0) < 0) {
        ERROR("%s: Couldn't open SDL_mixer audio device: %s", __FUNCTION__, Mix_GetError());
        SDL_Log("RP5NP_MIXER_OPEN_FAIL error=%s", Mix_GetError());
        return ERR_NOEFFECT;
    }

    int rp5np_mix_freq = 0, rp5np_mix_channels = 0;
    Uint16 rp5np_mix_format = 0;
    if (!Mix_QuerySpec(&rp5np_mix_freq, &rp5np_mix_format, &rp5np_mix_channels)) {
        SDL_Log("RP5NP_MIXER_QUERY_FAIL error=%s", Mix_GetError());
        Mix_CloseAudio();
        return ERR_NOEFFECT;
    }
    SDL_Log("RP5NP_MIXER_OPEN_OK freq=%d format=%u channels=%d",
            rp5np_mix_freq, rp5np_mix_format, rp5np_mix_channels);
    SDL_Log("RP5NP_AUDIO_ARCHITECTURE=SDL_MIXER_SINGLE_DEVICE");
    SDL_Log("RP5NP_AUDIO_BUFFER_FRAMES=1024 nominal_ms=21.333");

    Mix_AllocateChannels(SND_MAX_SAMPLES);
    Mix_HookMusic(MusicCallback, (void *)&MusicDev);
    Mix_SetPostMix(rp5np_external_stream_postmix, (void *)&cutscene_audiostream);
    Mix_VolumeMusic(MIX_MAX_VOLUME);

    InitReadXMI();
    atexit(Mix_CloseAudio);
    return OK;
#else
    SDL_AudioSpec spec, obtained;
    spec.freq = 48000;
    spec.format = AUDIO_S16SYS;
    spec.channels = 2;
    spec.samples = 2048;
    spec.callback = AudioStreamCallback;
    spec.userdata = (void *)&cutscene_audiostream;

    extern SDL_AudioDeviceID device;
    device = SDL_OpenAudioDevice(NULL, 0, &spec, &obtained, 0);

    if (device == 0) {
        ERROR("Could not open SDL audio: %s", SDL_GetError());
    } else {
        INFO("Opened Music Stream, deviceID %d, freq %d, size %d, format %d, channels %d, samples %d", device,
             obtained.freq, obtained.size, obtained.format, obtained.channels, obtained.samples);
    }

    int mix_init_flags = Mix_Init(MIX_INIT_MP3);
    if ((mix_init_flags & MIX_INIT_MP3) != MIX_INIT_MP3) {
        ERROR("%s: SDL_mixer MP3 init failed: %s", __FUNCTION__, Mix_GetError());
    }

    if (Mix_OpenAudio(48000, AUDIO_S16SYS, 2, 2048) < 0) {
        ERROR("%s: Couldn't open audio device: %s", __FUNCTION__, Mix_GetError());
    }

    Mix_AllocateChannels(SND_MAX_SAMPLES);
    Mix_HookMusic(MusicCallback, (void *)&MusicDev);
    Mix_VolumeMusic(MIX_MAX_VOLUME);

    InitReadXMI();

    atexit(Mix_CloseAudio);
    atexit(SDL_CloseAudio);

    return OK;
#endif
}

int snd_sample_play(int snd_ref, int len, uchar *smp, struct snd_digi_parms *dprm) {

    // Play one of the VOC format sounds

    Mix_Chunk *sample = Mix_LoadWAV_RW(SDL_RWFromConstMem(smp, len), 1);
    if (sample == NULL) {
        DEBUG("%s: Failed to load sample", __FUNCTION__);
        return ERR_NOEFFECT;
    }

    int loops = dprm->loops > 0 ? dprm->loops - 1 : -1;
    int channel = Mix_PlayChannel(-1, sample, loops);
    if (channel < 0) {
        DEBUG("%s: Failed to play sample", __FUNCTION__);
#ifdef __ANDROID__
        SDL_Log("RP5NP_GAME_SFX_PLAY_FAIL ref=%d error=%s", snd_ref, Mix_GetError());
#endif
        Mix_FreeChunk(sample);
        return ERR_NOEFFECT;
    }
#ifdef __ANDROID__
    if (!rp5np_first_sfx_logged) {
        rp5np_first_sfx_logged = true;
        SDL_Log("RP5NP_GAME_SFX_PLAY_OK ref=%d channel=%d", snd_ref, channel);
    }
#endif

    if (samples_by_channel[channel])
        Mix_FreeChunk(samples_by_channel[channel]);

    samples_by_channel[channel] = sample;
    digi_parms_by_channel[channel] = *dprm;
    snd_sample_reload_parms(&digi_parms_by_channel[channel]);

    return channel;
}

void snd_end_sample(int hnd_id) {
    Mix_HaltChannel(hnd_id);
    if (samples_by_channel[hnd_id]) {
        Mix_FreeChunk(samples_by_channel[hnd_id]);
        samples_by_channel[hnd_id] = NULL;
    }
}

bool snd_sample_playing(int hnd_id) { return Mix_Playing(hnd_id); }

snd_digi_parms *snd_sample_parms(int hnd_id) { return &digi_parms_by_channel[hnd_id]; }

void snd_kill_all_samples(void) {
    for (int channel = 0; channel < SND_MAX_SAMPLES; channel++) {
        snd_end_sample(channel);
    }

    // assume we want these too
    //    StopTheMusic(); // no, don't stop the music
    if (cutscene_audiostream != NULL)
        SDL_AudioStreamClear(cutscene_audiostream);
}

void snd_sample_reload_parms(snd_digi_parms *sdp) {
    // ignore if *sdp is not one of the items in digi_parms_by_channel[]
    if (sdp < digi_parms_by_channel || sdp > digi_parms_by_channel + SND_MAX_SAMPLES)
        return;
    int channel = sdp - digi_parms_by_channel;

    if (!Mix_Playing(channel))
        return;

    // sdp->vol ranges from 0..255
    Mix_Volume(channel, (sdp->vol * 128) / 100);

    // sdp->pan ranges from 1 (left) to 127 (right)
    uint8_t right = 2 * sdp->pan;
    Mix_SetPanning(channel, 254 - right, right);
}

int is_playing = 0;

int MacTuneLoadTheme(char *theme_base, int themeID) {
    char filename[40];
    FILE *f;
    int i;

#define NUM_SCORES 8
#define SUPERCHUNKS_PER_SCORE 4
#define NUM_TRANSITIONS 9
#define NUM_LAYERS 32
#define MAX_KEYS 10
#define NUM_LAYERABLE_SUPERCHUNKS 22
#define KEY_BAR_RESOLUTION 2

    extern uchar track_table[NUM_SCORES][SUPERCHUNKS_PER_SCORE];
    extern uchar transition_table[NUM_TRANSITIONS];
    extern uchar layering_table[NUM_LAYERS][MAX_KEYS];
    extern uchar key_table[NUM_LAYERABLE_SUPERCHUNKS][KEY_BAR_RESOLUTION];

    StopTheMusic();

    FreeXMI();

    if (strncmp(theme_base, "thm", 3)) {
        sprintf(filename, "res/sound/%s/%s.xmi", MusicDev->musicType, theme_base);
        ReadXMI(filename);
    } else {
        sprintf(filename, "res/sound/%s/thm%i.xmi", MusicDev->musicType, themeID);
        ReadXMI(filename);

        sprintf(filename, "res/sound/thm%i.bin", themeID);
        extern FILE *fopen_caseless(const char *path, const char *mode); // see caseless.c
        f = fopen_caseless(filename, "rb");
        if (f != 0) {
            fread(track_table, NUM_SCORES * SUPERCHUNKS_PER_SCORE, 1, f);
            fread(transition_table, NUM_TRANSITIONS, 1, f);
            fread(layering_table, NUM_LAYERS * MAX_KEYS, 1, f);
            fread(key_table, NUM_LAYERABLE_SUPERCHUNKS * KEY_BAR_RESOLUTION, 1, f);

            fclose(f);
        }
    }

    return OK;
}

void MacTuneKillCurrentTheme(void) { StopTheMusic(); }

#else

// Sound stubs that do nothing, when SDL Mixer is not found

int snd_start_digital(void) { return OK; }
int snd_sample_play(int snd_ref, int len, uchar *smp, struct snd_digi_parms *dprm) { return OK; }
int snd_alog_play(int snd_ref, int len, uchar *smp, struct snd_digi_parms *dprm) { return OK; }
void snd_end_sample(int hnd_id) {}
void snd_kill_all_samples(void) {}
int MacTuneLoadTheme(char *theme_base, int themeID) { return OK; }
void MacTuneKillCurrentTheme(void) {}
snd_digi_parms *snd_sample_parms(int hnd_id) { return &digi_parms_by_channel[0]; }
bool snd_sample_playing(int hnd_id) { return false; }
void snd_sample_reload_parms(snd_digi_parms *sdp) {}

#endif

// Unimplemented sound stubs

void snd_startup(void) {}
int snd_stop_digital(void) { return 1; }
