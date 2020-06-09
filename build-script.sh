!/bin/bash

if [ -z "$1" ]; then
  TARGET=macosx
else
  TARGET="$1"
fi

case $TARGET in
  macosx)
    VARIABLES_FILES="/etc/scons/xcode_macosx.vars"
  android-armv7)
    VARIABLES_FILES="/etc/scons/"
  android-arm64)
  android-x86)
  iOS)
  iOS-sim)
