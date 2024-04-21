# KDevelop

This repository contains the source code for the KDevelop IDE.
It also includes KDevelop Platform (kdevplatform) and most of the plugins.

Other KDevelop plugins can be found in [KDevelop Group](https://invent.kde.org/kdevelop) of KDE GitLab instance.

## User Documentation

User documentation is available from:
https://userbase.kde.org/KDevelop5/Manual

## Compile

KDevelop is built the same way as most KDE projects, using CMake to set up a build directory and build options.
For detailed instructions how to compile KDevelop, please refer to the Wiki:
https://community.kde.org/KDevelop/HowToCompile_v5 .

### Optional dependencies

Most of KDevelop's optional dependencies are opportunistic under the assumption that the build should
always use all available functionality, as well as the most recent version. CMake has a lesser known feature
to control which of such dependencies is used.

For instance, to skip building the Subversion plugin, use
`-DCMAKE_DISABLE_FIND_PACKAGE_SubversionLibrary=ON`. To use QtWebKit instead of QtWebEngine for
documentation rendering when both are available, add `-DCMAKE_DISABLE_FIND_PACKAGE_Qt5WebEngineWidgets=ON` to
the CMake arguments.

## Contribute

If you want to contribute to KDevelop, please read through:
https://www.kdevelop.org/contribute-kdevelop

## Development Infrastructure
- [Bug tracker](https://bugs.kde.org/buglist.cgi?bug_status=UNCONFIRMED&bug_status=CONFIRMED&bug_status=ASSIGNED&bug_status=REOPENED&list_id=1777266&product=kdevelop&product=kdevplatform&query_format=advanced)
- [KDE GitLab instance (code review as well as hosting and other important collaboration tasks)](https://invent.kde.org/kdevelop/kdevelop/-/merge_requests)
- [Phabricator (task tracker until this functionality is migrated to GitLab)](https://phabricator.kde.org/dashboard/view/8/?)

## Chuck's notes
Building from source on MX Linux requires installing most of the KDE development tools.

```apt-get install -y libbz2-dev libxslt-dev libxml2-dev shared-mime-info oxygen-icon-theme libgif-dev libvlc-dev libvlccore-dev doxygen gperf bzr libxapian-dev fontforge libgcrypt20-dev libattr1-dev network-manager-dev libgtk-3-dev xsltproc xserver-xorg-dev xserver-xorg-input-synaptics-dev libpwquality-dev modemmanager-dev libxcb-keysyms1-dev libepoxy-dev libpolkit-agent-1-dev libegl1-mesa-dev libxcb-xkb-dev libqt5x11extras5-dev libwww-perl libxml-parser-perl libjson-perl libboost-dev libgstreamer-plugins-base1.0-dev libgstreamer1.0-dev libarchive-dev liblmdb-dev cmake git libkomparediff2-dev extra-cmake-modules "libkf5.*-dev" llvm libclang-dev```

The libraries ```libnm-util-dev libnm-glib-dev``` are not available as of MX 23.2.
