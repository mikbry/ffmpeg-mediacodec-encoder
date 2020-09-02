#!/bin/bash

# If no FFmpeg clone it
function cloneFFmpeg() {
  if [ ! -d ./ffmpeg ]; then
      git clone https://git.ffmpeg.org/ffmpeg.git ffmpeg
  fi
}

cloneFFmpeg
