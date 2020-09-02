# ffmpeg-mediacodec-encoder
[WIP] FFMPEG hardware encoder for Android

## Why ?
- hardware support on Android is only available for decoder
- libx264 and other sw encoders are slow on Android

## Sources
- https://trac.ffmpeg.org/wiki/HWAccelIntro
- https://github.com/FFmpeg/FFmpeg/search?p=1&q=mediacodec&unscoped_q=mediacodec
- https://github.com/tanersener/mobile-ffmpeg/issues/225
- http://ogorkis.net/ffmpeg (Old and use deprecated functions)

## TODO
- [x] Create an android project
- [ ] add FFMPEG lib
- [ ] create a stub C encoder
- [ ] implement Mediacodec encoder

## How to

### Build FFmpeg for Android

https://github.com/madhavanmalolan/ffmpegandroidlibrary/wiki/Building-from-source

https://github.com/Javernaut/ffmpeg-android-maker/blob/master/ffmpeg-android-maker.sh

https://yesimroy.gitbooks.io/android-note/content/compile_ffmpeg_for_android.html

https://syllogismobile.wordpress.com/2018/08/19/building-ffmpeg-4-0-for-android-with-clang/

https://github.com/tanersener/mobile-ffmpeg/blob/master/android.sh

https://github.com/binglingziyu/ffmpeg-android-build

Create a script to:
- [x] Clone FFmpeg from git
- [ ] Configure NDK
- [ ] Setup toolchain(s)
- [ ] Configure FFmpeg
- [ ] Build
### Create a minimal FFMPEG H264 encoder
Based on [Videotoolbox encoder](https://github.com/FFmpeg/FFmpeg/blob/a72d5290c5b70780fe7132c68498c0aa1456908f/libavcodec/videotoolboxenc.c)

``` c
//  filename: mediacodecenc.c

static const AVClass h264_mediacodec_class = {
    .class_name = "h264_mediacodec",
    .item_name  = av_default_item_name,
    .option     = h264_options,
    .version    = LIBAVUTIL_VERSION_INT,
};

AVCodec ff_h264_mediacodec_encoder = {
    .name             = "h264_mediacodec",
    .long_name        = NULL_IF_CONFIG_SMALL("Mediacodec H.264 Encoder"),
    .type             = AVMEDIA_TYPE_VIDEO,
    .id               = AV_CODEC_ID_H264,
    .priv_data_size   = sizeof(VTEncContext),
    .pix_fmts         = avc_pix_fmts,
    .init             = vtenc_init,
    .encode2          = vtenc_frame,
    .close            = vtenc_close,
    .capabilities     = AV_CODEC_CAP_DELAY,
    .priv_class       = &h264_mediacodec_class,
    .caps_internal    = FF_CODEC_CAP_INIT_THREADSAFE |
                        FF_CODEC_CAP_INIT_CLEANUP,
};

```