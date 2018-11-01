/*
  SDL_mixer:  An audio mixer library based on the SDL library
  Copyright (C) 1997-2018 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "mixer.h"
#ifdef MUSIC_OPUS

/* This file supports Ogg Opus music streams */

#include "SDL_loadso.h"

#include "music_opus.h"

#include <opusfile.h>

typedef struct {
    int loaded;
    void *handle;
    const OpusTags *(*op_tags)(const OggOpusFile *,int);
    OggOpusFile *(*op_open_callbacks)(void *,const OpusFileCallbacks *,const unsigned char *,size_t,int *);
    void (*op_free)(OggOpusFile *);
    const OpusHead *(*op_head)(const OggOpusFile *,int);
    int (*op_seekable)(const OggOpusFile *);
    int (*op_read)(OggOpusFile *, opus_int16 *,int,int *);
    int (*op_pcm_seek)(OggOpusFile *,ogg_int64_t);
    ogg_int64_t (*op_pcm_tell)(const OggOpusFile *);
    ogg_int64_t (*op_pcm_total)(const OggOpusFile *, int);
} opus_loader;

static opus_loader opus = {
    0, NULL
};

#ifdef OPUS_DYNAMIC
#define FUNCTION_LOADER(FUNC, SIG) \
    opus.FUNC = (SIG) SDL_LoadFunction(opus.handle, #FUNC); \
    if (opus.FUNC == NULL) { SDL_UnloadObject(opus.handle); return -1; }
#else
#define FUNCTION_LOADER(FUNC, SIG) \
    opus.FUNC = FUNC;
#endif

static int OPUS_Load(void)
{
    if (opus.loaded == 0) {
#ifdef OPUS_DYNAMIC
        opus.handle = SDL_LoadObject(OPUS_DYNAMIC);
        if (opus.handle == NULL) {
            return -1;
        }
#elif defined(__MACOSX__)
        extern OggOpusFile *op_open_callbacks(void *,const OpusFileCallbacks *,const unsigned char *,size_t,int *) __attribute__((weak_import));
        if (op_open_callbacks == NULL) {
            /* Missing weakly linked framework */
            Mix_SetError("Missing Opus.framework");
            return -1;
        }
#endif
        FUNCTION_LOADER(op_open_callbacks, OggOpusFile *(*)(void *,const OpusFileCallbacks *,const unsigned char *,size_t,int *))
        FUNCTION_LOADER(op_tags, const OpusTags *(*)(const OggOpusFile *,int))
        FUNCTION_LOADER(op_free, void (*)(OggOpusFile *))
        FUNCTION_LOADER(op_head, const OpusHead *(*)(const OggOpusFile *,int))
        FUNCTION_LOADER(op_seekable, int (*)(const OggOpusFile *))
        FUNCTION_LOADER(op_read, int (*)(OggOpusFile *, opus_int16 *,int,int *))
        FUNCTION_LOADER(op_pcm_seek, int (*)(OggOpusFile *,ogg_int64_t))
        FUNCTION_LOADER(op_pcm_tell, ogg_int64_t (*)(const OggOpusFile *))
        FUNCTION_LOADER(op_pcm_total, ogg_int64_t (*)(const OggOpusFile *, int))
    }
    ++opus.loaded;

    return 0;
}

static void OPUS_Unload(void)
{
    if (opus.loaded == 0) {
        return;
    }
    if (opus.loaded == 1) {
#ifdef OPUS_DYNAMIC
        SDL_UnloadObject(opus.handle);
#endif
    }
    --opus.loaded;
}


typedef struct {
    SDL_RWops *src;
    int freesrc;
    int play_count;
    int volume;
    OggOpusFile *of;
    const OpusHead *op_info;
    int section;
    SDL_AudioStream *stream;
    char *buffer;
    int buffer_size;
    int loop;
    ogg_int64_t loop_start;
    ogg_int64_t loop_end;
    ogg_int64_t loop_len;
    ogg_int64_t channels;
    Mix_MusicMetaTags tags;
} OPUS_music;


static int set_op_error(const char *function, int error)
{
#define HANDLE_ERROR_CASE(X)    case X: Mix_SetError("%s: %s", function, #X); break;
    switch (error) {
    HANDLE_ERROR_CASE(OP_FALSE);
    HANDLE_ERROR_CASE(OP_EOF);
    HANDLE_ERROR_CASE(OP_HOLE);
    HANDLE_ERROR_CASE(OP_EREAD);
    HANDLE_ERROR_CASE(OP_EFAULT);
    HANDLE_ERROR_CASE(OP_EIMPL);
    HANDLE_ERROR_CASE(OP_EINVAL);
    HANDLE_ERROR_CASE(OP_ENOTFORMAT);
    HANDLE_ERROR_CASE(OP_EBADHEADER);
    HANDLE_ERROR_CASE(OP_EVERSION);
    HANDLE_ERROR_CASE(OP_ENOTAUDIO);
    HANDLE_ERROR_CASE(OP_EBADPACKET);
    HANDLE_ERROR_CASE(OP_EBADLINK);
    HANDLE_ERROR_CASE(OP_ENOSEEK);
    HANDLE_ERROR_CASE(OP_EBADTIMESTAMP);
    default:
        Mix_SetError("%s: unknown error %d\n", function, error);
        break;
    }
    return -1;
}

static int sdl_read_func(void *datasource, unsigned char *ptr, int size)
{
    return (int)SDL_RWread((SDL_RWops*)datasource, ptr, 1, size);
}

static int sdl_seek_func(void *datasource, opus_int64 offset, int whence)
{
    return (SDL_RWseek((SDL_RWops*)datasource, offset, whence) < 0)? -1 : 0;
}

static opus_int64 sdl_tell_func(void *datasource)
{
    return SDL_RWtell((SDL_RWops*)datasource);
}

static int OPUS_Seek(void*, double);
static void OPUS_Delete(void*);

static int OPUS_UpdateSection(OPUS_music *music)
{
    const OpusHead *op_info;

    op_info = opus.op_head(music->of, -1);
    if (!op_info) {
        Mix_SetError("op_head returned NULL");
        return -1;
    }

    if (music->op_info && op_info->channel_count == music->op_info->channel_count) {
        return 0;
    }
    music->op_info = op_info;

    if (music->buffer) {
        SDL_free(music->buffer);
        music->buffer = NULL;
    }

    if (music->stream) {
        SDL_FreeAudioStream(music->stream);
        music->stream = NULL;
    }

    music->stream = SDL_NewAudioStream(AUDIO_S16, op_info->channel_count, 48000,
                                       music_spec.format, music_spec.channels, music_spec.freq);
    if (!music->stream) {
        return -1;
    }

    music->buffer_size = music_spec.samples * sizeof(opus_int16) * op_info->channel_count;
    music->buffer = (char *)SDL_malloc(music->buffer_size);
    if (!music->buffer) {
        return -1;
    }
    return 0;
}

/* Convert string into integer with clean-up from junk and leading zeroes */
static ogg_int64_t str_to_ogg_int64(char *param)
{
    char *front = param;
    char *back = param;

    /* Find digit between of 1 and 9 at begin */
    while ( (*front != '\0') && ((*front < '1') || (*front > '9')) )
        front++;

    /* Find any non-digit character or NULL */
    back = front;
    while ((*back != '\0') && ((*back >= '0') && (*back <= '9')))
        back++;

    /* Put a string terminator into back just in case */
    *back = '\0';

    return (ogg_int64_t)SDL_strtoull(front, NULL, 0);
}

/* Load an Opus stream from an SDL_RWops object */
static void *OPUS_CreateFromRW(SDL_RWops *src, int freesrc)
{
    OPUS_music *music;
    OpusFileCallbacks callbacks;
    const OpusTags* tags = NULL;
    int err = 0, ci, isLoopLength = 0;
    ogg_int64_t fullLength;

    music = (OPUS_music *)SDL_calloc(1, sizeof *music);
    if (!music) {
        SDL_OutOfMemory();
        return NULL;
    }
    music->src = src;
    music->volume = MIX_MAX_VOLUME;
    music->section = -1;
    music->loop = -1;
    music->loop_start = -1;
    music->loop_end = 0;
    music->loop_len = 0;

    SDL_zero(callbacks);
    callbacks.read = sdl_read_func;
    callbacks.seek = sdl_seek_func;
    callbacks.tell = sdl_tell_func;

    music->of = opus.op_open_callbacks(src, &callbacks, NULL, 0, &err);
    if (music->of == NULL) {
    /*  set_op_error("op_open_callbacks", err);*/
        SDL_SetError("Not an Opus audio stream");
        SDL_free(music);
        return NULL;
    }

    if (!opus.op_seekable(music->of)) {
        OPUS_Delete(music);
        Mix_SetError("Opus stream not seekable");
        return NULL;
    }

    if (OPUS_UpdateSection(music) < 0) {
        OPUS_Delete(music);
        return NULL;
    }

    tags = opus.op_tags(music->of, -1);
    for (ci=0; ci<tags->comments; ci++) {
        char *param = SDL_strdup(tags->user_comments[ci]);
        char *argument = param;
        char *value = SDL_strchr(param, '=');
        if (value == NULL) {
            value = param + SDL_strlen(param);
        } else {
            *(value++) = '\0';
        }

        if (SDL_strcasecmp(argument, "LOOPSTART") == 0)
            music->loop_start = str_to_ogg_int64(value);
        else if (SDL_strcasecmp(argument, "LOOPLENGTH") == 0) {
            music->loop_len = str_to_ogg_int64(value);
            isLoopLength = 1;
        } else if (SDL_strcasecmp(argument, "LOOPEND") == 0) {
            isLoopLength = 0;
            music->loop_end = str_to_ogg_int64(value);
        } else if (SDL_strcasecmp(argument, "TITLE") == 0) {
            meta_tags_set(&music->tags, MIX_META_TITLE, value);
        } else if (SDL_strcasecmp(argument, "ARTIST") == 0) {
            meta_tags_set(&music->tags, MIX_META_ARTIST, value);
        } else if (SDL_strcasecmp(argument, "ALBUM") == 0) {
            meta_tags_set(&music->tags, MIX_META_ALBUM, value);
        } else if (SDL_strcasecmp(argument, "COPYRIGHT") == 0) {
            meta_tags_set(&music->tags, MIX_META_COPYRIGHT, value);
        }
        SDL_free(param);
    }

    if (isLoopLength == 1) {
        music->loop_end = music->loop_start + music->loop_len;
    } else {
        music->loop_len = music->loop_end - music->loop_start;
    }

    fullLength = opus.op_pcm_total(music->of, -1);
    if (((music->loop_start >= 0) || (music->loop_end > 0)) &&
        ((music->loop_start < music->loop_end) || (music->loop_end == 0)) &&
         (music->loop_start < fullLength) &&
         (music->loop_end <= fullLength)) {
        if (music->loop_start < 0) music->loop_start = 0;
        if (music->loop_end == 0)  music->loop_end = fullLength;
        music->loop = 1;
    }

    music->freesrc = freesrc;
    return music;
}

static const char* OPUS_GetMetaTag(void *context, Mix_MusicMetaTag tag_type)
{
    OPUS_music *music = (OPUS_music *)context;
    return meta_tags_get(&music->tags, tag_type);
}

/* Set the volume for an Opus stream */
static void OPUS_SetVolume(void *context, int volume)
{
    OPUS_music *music = (OPUS_music *)context;
    music->volume = volume;
}

/* Start playback of a given Opus stream */
static int OPUS_Play(void *context, int play_count)
{
    OPUS_music *music = (OPUS_music *)context;
    music->play_count = play_count;
    return OPUS_Seek(music, 0.0);
}

/* Play some of a stream previously started with OPUS_Play() */
static int OPUS_GetSome(void *context, void *data, int bytes, SDL_bool *done)
{
    OPUS_music *music = (OPUS_music *)context;
    SDL_bool looped = SDL_FALSE;
    int filled, samples, section, result;
    ogg_int64_t pcmPos;

    filled = SDL_AudioStreamGet(music->stream, data, bytes);
    if (filled != 0) {
        return filled;
    }

    if (!music->play_count) {
        /* All done */
        *done = SDL_TRUE;
        return 0;
    }

    section = music->section;
    samples = opus.op_read(music->of, (opus_int16 *)music->buffer, music->buffer_size / sizeof(opus_int16), &section);
    if (samples < 0) {
        set_op_error("op_read", samples);
        return -1;
    }

    if (section != music->section) {
        music->section = section;
        if (OPUS_UpdateSection(music) < 0) {
            return -1;
        }
    }

    pcmPos = opus.op_pcm_tell(music->of);
    if ((music->loop == 1) && (music->play_count != 0) && (pcmPos >= music->loop_end)) {
        samples -= (int)((pcmPos - music->loop_end) * music->channels) * (int)sizeof(Sint16);
        result = opus.op_pcm_seek(music->of, music->loop_start);
        if (result < 0) {
            set_op_error("ov_pcm_seek", result);
            return -1;
        } else {
            int play_count = -1;
            if (music->play_count > 0) {
                play_count = (music->play_count - 1);
            }
            music->play_count = play_count;
        }
        looped = SDL_TRUE;
    }

    if (samples > 0) {
        filled = samples * music->op_info->channel_count * 2;
        if (SDL_AudioStreamPut(music->stream, music->buffer, filled) < 0) {
            return -1;
        }
    } else if (!looped) {
        if (music->play_count == 1) {
            music->play_count = 0;
            SDL_AudioStreamFlush(music->stream);
        } else {
            int play_count = -1;
            if (music->play_count > 0) {
                play_count = (music->play_count - 1);
            }
            if (OPUS_Play(music, play_count) < 0) {
                return -1;
            }
        }
    }
    return 0;
}

static int OPUS_GetAudio(void *context, void *data, int bytes)
{
    OPUS_music *music = (OPUS_music *)context;
    return music_pcm_getaudio(context, data, bytes, music->volume, OPUS_GetSome);
}

/* Jump (seek) to a given position (time is in seconds) */
static int OPUS_Seek(void *context, double time)
{
    OPUS_music *music = (OPUS_music *)context;
    int result;
    result = opus.op_pcm_seek(music->of, (ogg_int64_t)(time * 48000));
    if (result < 0) {
        return set_op_error("op_pcm_seek", result);
    }
    return 0;
}

static double OPUS_Tell(void *context)
{
    OPUS_music *music = (OPUS_music *)context;
    return (double)opus.op_pcm_tell(music->of) / 48000.0;
}

static double OPUS_Total(void *context)
{
    OPUS_music *music = (OPUS_music *)context;
    return (double)opus.op_pcm_total(music->of, -1) / 48000.0;
}

static double OPUS_get_loop_start(void *music_p)
{
    OPUS_music *music = (OPUS_music *)music_p;
    if (music->loop) {
        return (double)music->loop_start / 48000.0;
    }
    return -1.0;
}

static double OPUS_get_loop_end(void *music_p)
{
    OPUS_music *music = (OPUS_music *)music_p;
    if (music->loop) {
        return (double)music->loop_end / 48000.0;
    }
    return -1.0;
}

static double OPUS_get_loop_length(void *music_p)
{
    OPUS_music *music = (OPUS_music *)music_p;
    if (music->loop) {
        return (double)music->loop_len / 48000.0;
    }
    return -1.0;
}

/* Close the given Opus stream */
static void OPUS_Delete(void *context)
{
    OPUS_music *music = (OPUS_music *)context;
    opus.op_free(music->of);
    if (music->stream) {
        SDL_FreeAudioStream(music->stream);
    }
    if (music->buffer) {
        SDL_free(music->buffer);
    }
    if (music->freesrc) {
        SDL_RWclose(music->src);
    }
    SDL_free(music);
}

Mix_MusicInterface Mix_MusicInterface_Opus =
{
    "OPUS",
    MIX_MUSIC_OPUS,
    MUS_OPUS,
    SDL_FALSE,
    SDL_FALSE,

    OPUS_Load,
    NULL,   /* Open */
    OPUS_CreateFromRW,
    NULL,   /* CreateFromRWex [MIXER-X]*/
    NULL,   /* CreateFromFile */
    NULL,   /* CreateFromFileEx [MIXER-X]*/
    OPUS_SetVolume,
    OPUS_Play,
    NULL,   /* IsPlaying */
    OPUS_GetAudio,
    OPUS_Seek,
    OPUS_Tell, /* Tell [MIXER-X]*/
    OPUS_Total, /* FullLength [MIXER-X]*/
    OPUS_get_loop_start, /* LoopStart [MIXER-X]*/
    OPUS_get_loop_end, /* LoopEnd [MIXER-X]*/
    OPUS_get_loop_length, /* LoopLength [MIXER-X]*/
    OPUS_GetMetaTag, /* GetMetaTag [MIXER-X]*/
    NULL,   /* Pause */
    NULL,   /* Resume */
    NULL,   /* Stop */
    OPUS_Delete,
    NULL,   /* Close */
    OPUS_Unload,
};

#endif /* MUSIC_OPUS */

/* vi: set ts=4 sw=4 expandtab: */