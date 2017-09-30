#APP_PROJECT_PATH := 
#APP_MODULES := 

############################################################
#release
#debug
############################################################ 
APP_OPTIM := release

#APP_CFLAGS := 
#APP_CXXFLAGS := 
#APP_CPPFLAGS := 
#APP_LDFLAGS := 
#APP_BUILD_SCRIPT := 

############################################################
#armeabi
#armeabi-v7a
#x86
#mips
#all
############################################################
APP_ABI := armeabi armeabi-v7a

############################################################
#android-3      -> Official Android 1.5 system images
#android-4      -> Official Android 1.6 system images
#android-5      -> Official Android 2.0 system images
#android-6      -> Official Android 2.0.1 system images
#android-7      -> Official Android 2.1 system images
#android-8      -> Official Android 2.2 system images
#android-9      -> Official Android 2.3 system images
#android-14     -> Official Android 4.0 system images
#android-18     -> Official Android 4.3 system images
############################################################
APP_PLATFORM := android-9

############################################################
#system          -> Use the default minimal system C++ runtime library.
#gabi++_static   -> Use the GAbi++ runtime as a static library.
#gabi++_shared   -> Use the GAbi++ runtime as a shared library.
#stlport_static  -> Use the STLport runtime as a static library.
#stlport_shared  -> Use the STLport runtime as a shared library.
#gnustl_static   -> Use the GNU STL as a static library.
#gnustl_shared   -> Use the GNU STL as a shared library.
#c++_static      -> Use the LLVM libc++ as a static library.
#c++_shared      -> Use the LLVM libc++ as a shared library.
############################################################
#APP_STL := gnustl_static

APP_STL := gnustl_static

#APP_GNUSTL_FORCE_CPP_FEATURES := 
#APP_SHORT_COMMANDS := 
#NDK_TOOLCHAIN_VERSION := 4.7 
#APP_PIE := 
#APP_THIN_ARCHIVE := 
APP_CPPFLAGS := -frtti -std=c++11
