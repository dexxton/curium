<<<<<<< HEAD
packages:=boost openssl libevent zeromq
native_packages := native_ccache
=======
packages:=boost openssl libevent
darwin_packages:=zeromq
linux_packages:=zeromq
native_packages := native_ccache native_comparisontool
>>>>>>> dev-1.12.1.0

qt_native_packages = native_protobuf
qt_packages = qrencode protobuf zlib

<<<<<<< HEAD
qt_x86_64_linux_packages:=qt expat dbus libxcb xcb_proto libXau xproto freetype fontconfig libX11 xextproto libXext xtrans
qt_i686_linux_packages:=$(qt_x86_64_linux_packages)
qt_arm_linux_packages:=$(qt_x86_64_linux_packages)
qt_aarch64_linux_packages:=$(qt_x86_64_linux_packages)

qt_darwin_packages=qt
qt_mingw32_packages=qt

=======
qt_linux_packages= qt expat dbus libxcb xcb_proto libXau xproto freetype fontconfig libX11 xextproto libXext xtrans
qt_darwin_packages=qt
qt_mingw32_packages=qt


>>>>>>> dev-1.12.1.0
wallet_packages=bdb

upnp_packages=miniupnpc

darwin_native_packages = native_biplist native_ds_store native_mac_alias

ifneq ($(build_os),darwin)
<<<<<<< HEAD
darwin_native_packages += native_cctools native_cdrkit native_libdmg-hfsplus
=======
darwin_native_packages=native_cctools native_cdrkit native_libdmg-hfsplus
>>>>>>> dev-1.12.1.0
endif
