#!/bin/bash

if [ -z "$1" ]; then
  J=4
else
  J="$1"
fi

if [ -z "$2" ]; then
  TARGET=macosx
else
  TARGET="$2"
fi

function run_macosx {
  if [[ "$OSTYPE" != "darwin"* ]]; then
    echo "Must run on macosx"
  else
    ./buildscripts/scons.py --link-model=dynamic-sdk --install-mode=hygienic --disable-warnings-as-errors --enable-free-mon=off --js-engine=none --dbg=off --opt=size --wiredtiger=off --use-system-mongo-c=off --allocator=system --lto install-embedded-addon -j$J MONGO_VERSION=4.0.4
  fi
}

func

case $TARGET in
  macosx)
    run_macosx
    ;;
  android)
    if [ -z "$3" ]; then
      echo "Must specify ndk path"
    else
      NDK_PATH="$3"
      if [ -z "$4" ]; then
        ARCH="armv7a"
      else
        ARCH="$4"
      fi
      if [[ "$OSTYPE" == "darwin"* ]]; then
        TOOLCHAIN_OS="darwin-x86_64"
      else
        TOOLCHAIN_OS="linux-x86_64"
      fi
      ./buildscripts/scons.py TARGET_OS="android" --link-model=dynamic-sdk --install-mode=hygienic --disable-warnings-as-errors --enable-free-mon=off --js-engine=none --dbg=off --opt=size --wiredtiger=off --use-system-mongo-c=off --allocator=system --lto install-embedded-addon -j$J CC=$NDK_PATH/toolchains/llvm/prebuilt/$TOOLCHAIN_OS/bin/$ARCH-linux-androideabi21-clang CXX=$NDK_PATH/toolchains/llvm/prebuilt/$TOOLCHAIN_OS/bin/$ARCH-linux-androideabi21-clang++ AR=$NDK_PATH/toolchains/llvm/prebuilt/$TOOLCHAIN_OS/bin/arm-linux-androideabi-ar MONGO_VERSION=4.0.4
    fi
    ;;
  iOS)
    ./buildscripts/scons.py --variables-files=etc/scons/xcode_ios.vars --link-model=dynamic-sdk --install-mode=hygienic --disable-warnings-as-errors --enable-free-mon=off --js-engine=none --dbg=off --opt=size --wiredtiger=off --use-system-mongo-c=off --allocator=system --lto install-embedded-addon -j$J MONGO_VERSION=4.0.4
    ;;
  iOS-sim)
    ./buildscripts/scons.py --variables-files=etc/scons/xcode_ios_sim.vars --link-model=dynamic-sdk --install-mode=hygienic --disable-warnings-as-errors --enable-free-mon=off --js-engine=none --dbg=off --opt=size --wiredtiger=off --use-system-mongo-c=off --allocator=system --lto install-embedded-addon -j$J MONGO_VERSION=4.0.4
esac
