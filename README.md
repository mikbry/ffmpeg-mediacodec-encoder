# ffmpeg-mediacodec-encoder
[experimental] FFMPEG hardware encoder for Android

## Why ?
- hardware support on Android is only available for decoder
- libx264 and other sw encoders are slow on Android

## Sources
- https://trac.ffmpeg.org/wiki/HWAccelIntro
- https://github.com/FFmpeg/FFmpeg/search?p=1&q=mediacodec&unscoped_q=mediacodec
- https://github.com/tanersener/mobile-ffmpeg/issues/225

## TODO
- [ ] Create an android project
- [ ] add FFMPEG lib
- [ ] create a stub C encoder
- [ ] implement Mediacodec encoder
