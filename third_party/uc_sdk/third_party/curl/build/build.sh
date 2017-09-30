#!/bin/sh

APP_ABI="armeabi"

PLATFORM=9

TARGET=arm-eabi

TOOLCHAIN_VERSION=4.8

NDK_ROOT=/home/ubuntu/dev/android-ndk-r10c

#if [[ "$APP_ABI" == "armeabi" ]] || [[ "$APP_ABI" == "armeabi-v7a" ]]; then
GCC_NAME="arm-linux-androideabi-gcc"
TOOL_CHAIN="arm-linux-androideabi"
#elif [ "$APP_ABI" == "x86" ]; then
#	TOOL_CHAIN="x86"
#	GCC_NAME="i686-linux-android-gcc"
#elif [ "$APP_ABI" == "mips" ]; then
#	GCC_NAME="mipsel-linux-android-gcc"
#	TOOL_CHAIN="mipsel-linux-android"
#fi


TOOL_CHAIN=${TOOL_CHAIN}-${TOOLCHAIN_VERSION}
PATH=$NDK_ROOT/toolchains/$TOOL_CHAIN/prebuilt/linux-x86/bin:$PATH

LIBGCC=$NDK_ROOT/toolchains/$TOOL_CHAIN/prebuilt/linux-x86/lib/gcc/arm-linux-androideabi/$TOOLCHAIN_VERSION/thumb
STCLIB=$NDK_ROOT/platforms/android-$PLATFORM/arch-arm/usr/lib
SYSROOT=$NDK_ROOT/platforms/android-$PLATFORM/arch-arm/
#OPENSSL_PATH=/mnt/disk/android-ndk-r10d/ssl/android-14
CFLAGS_V="-fpic -ffunction-sections -funwind-tables -fstack-protector \
-no-canonical-prefixes -march=armv7-a -mfpu=vfpv3-d16 -mfloat-abi=softfp -mthumb \
-fomit-frame-pointer -fno-strict-aliasing -finline-limit=64 \
-FT_PLATFORM=FT_PLATFORM_ANDROID -DANDROID -I$NDK_ROOT/platforms/android-9/arch-arm/usr/include --sysroot=${SYSROOT}"

echo "hello ${CFLAGS_V}"

GCC_NAME=$NDK_ROOT/toolchains/$TOOL_CHAIN/prebuilt/linux-x86/bin/${GCC_NAME}
cd ..
./configure CC=${GCC_NAME} --host=arm-linux-androideabi \
	--with-ssl=/mnt/disk/android-ndk-r10d/ssl/android-14 \
	--enable-proxy \
	--enable-http \
	--disable-ftp \
	--disable-gopher \
	--disable-file \
	--disable-imap \
	--disable-ldap \
	--disable-ldaps \
	--disable-pop3 \
	--disable-rtsp \
	--disable-smtp \
	--disable-telnet \
	--disable-tftp \
	--without-gnutls \
	--without-libidn \
	--without-librtmp \
	--disable-dict \
	CPPFLAGS="${CFLAGS_V}" \
	CFLAGS="${CFLAGS_V}" \
	LIBS="-lgcc -lc" \
	LDFLAGS="-L$STCLIB -L$LIBGCC"

