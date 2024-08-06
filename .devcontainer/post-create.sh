#!/usr/bin/env bash

set -euxo pipefail

PS4='+${BASH_SOURCE[0]}:$LINENO: '
if [[ -t 1 ]] && type -t tput >/dev/null; then
  if (( "$(tput colors)" == 256 )); then
    PS4='$(tput setaf 10)'$PS4'$(tput sgr0)'
  else
    PS4='$(tput setaf 2)'$PS4'$(tput sgr0)'
  fi
fi

GMP_URL='https://ftp.gnu.org/gnu/gmp/gmp-6.3.0.tar.xz'
ABSEIL_CPP_URL="https://github.com/abseil/abseil-cpp/releases/download/20240116.2/abseil-cpp-20240116.2.tar.gz"
PROTOBUF_URL="https://github.com/protocolbuffers/protobuf/releases/download/v27.2/protobuf-27.2.tar.gz"
NIST_STASTICAL_TEST_SUITE_URL="https://csrc.nist.gov/CSRC/media/Projects/Random-Bit-Generation/documents/sts-2_1_2.zip"

# Install prerequisite packages.
sudo apt-get -y update
sudo apt-get -y dist-upgrade
sudo apt-get -y install \
    m4
sudo apt-get clean
sudo rm -rf /var/lib/apt/lists/*
sudo chown vscode:vscode /workspaces

pushd /workspaces
git clone https://github.com/Cryolite/prerequisites
popd

# Install GCC.
/workspaces/prerequisites/gcc/install --debug --prefix "$HOME/.local"
echo 'export C_INCLUDE_PATH="$HOME/.local/include${C_INCLUDE_PATH:+:$C_INCLUDE_PATH}"' >> "$HOME/.bashrc"
echo 'export C_INCLUDE_PATH="$HOME/.local/include${C_INCLUDE_PATH:+:$C_INCLUDE_PATH}"' >> "$HOME/.profile"
export C_INCLUDE_PATH="$HOME/.local/include${C_INCLUDE_PATH:+:$C_INCLUDE_PATH}"
echo 'export CPLUS_INCLUDE_PATH="$HOME/.local/include${CPLUS_INCLUDE_PATH:+:$CPLUS_INCLUDE_PATH}"' >> "$HOME/.bashrc"
echo 'export CPLUS_INCLUDE_PATH="$HOME/.local/include${CPLUS_INCLUDE_PATH:+:$CPLUS_INCLUDE_PATH}"' >> "$HOME/.profile"
export CPLUS_INCLUDE_PATH="$HOME/.local/include${CPLUS_INCLUDE_PATH:+:$CPLUS_INCLUDE_PATH}"
echo 'export LIBRARY_PATH="$HOME/.local/lib64:$HOME/.local/lib${LIBRARY_PATH:+:$LIBRARY_PATH}"' >> "$HOME/.bashrc"
echo 'export LIBRARY_PATH="$HOME/.local/lib64:$HOME/.local/lib${LIBRARY_PATH:+:$LIBRARY_PATH}"' >> "$HOME/.profile"
export LIBRARY_PATH="$HOME/.local/lib64:$HOME/.local/lib${LIBRARY_PATH:+:$LIBRARY_PATH}"
echo 'export LD_LIBRARY_PATH="$HOME/.local/lib64:$HOME/.local/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"' >> "$HOME/.bashrc"
echo 'export LD_LIBRARY_PATH="$HOME/.local/lib64:$HOME/.local/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"' >> "$HOME/.profile"
export LD_LIBRARY_PATH="$HOME/.local/lib64:$HOME/.local/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
echo 'export PATH="$HOME/.local/bin${PATH:+:$PATH}"' >> "$HOME/.bashrc"
echo 'export PATH="$HOME/.local/bin${PATH:+:$PATH}"' >> "$HOME/.profile"
export PATH="$HOME/.local/bin${PATH:+:$PATH}"
echo 'export CC="$HOME/.local/bin/gcc"' >> "$HOME/.bashrc"
echo 'export CC="$HOME/.local/bin/gcc"' >> "$HOME/.profile"
export CC="$HOME/.local/bin/gcc"
echo 'export CXX="$HOME/.local/bin/g++"' >> "$HOME/.bashrc"
echo 'export CXX="$HOME/.local/bin/g++"' >> "$HOME/.profile"
export CXX="$HOME/.local/bin/g++"

# Workaround
sed -i -e 's/make check/echo make check/' /workspaces/prerequisites/libbacktrace/install
/workspaces/prerequisites/libbacktrace/install --debug --prefix "$HOME/.local"

/workspaces/prerequisites/cmake/install --debug --prefix "$HOME/.local"

# Install GMP.
pushd /workspaces
GMP_TARBALL_NAME="$(basename "$GMP_URL")"
curl -fLsSo "$GMP_TARBALL_NAME" "$GMP_URL"
tar -xaf "$GMP_TARBALL_NAME"
rm -f "$GMP_TARBALL_NAME"
GMP_ROOT="$(readlink -e "$(basename "$GMP_TARBALL_NAME" .tar.xz)")"
GMP_BUILD_DIR="${GMP_ROOT}-build"
mkdir -p "$GMP_BUILD_DIR"
pushd "$GMP_BUILD_DIR"
"$GMP_ROOT/configure" "--prefix=$HOME/.local"
make -j$(nproc)
make check
make install
popd
rm -rf "$GMP_BUILD_DIR"
rm -rf "$GMP_ROOT"
popd

# Install Protocol Buffers.
pushd /workspaces
ABSEIL_TARBALL_NAME="$(basename "$ABSEIL_CPP_URL")"
curl -fLsSo "$ABSEIL_TARBALL_NAME" "$ABSEIL_CPP_URL"
tar -xaf "$ABSEIL_TARBALL_NAME"
rm -f "$ABSEIL_TARBALL_NAME"
ABSEIL_CPP_ROOT="$(readlink -e "$(basename "$ABSEIL_TARBALL_NAME" .tar.gz)")"
PROTOBUF_TARBALL_NAME="$(basename "$PROTOBUF_URL")"
curl -fLsSo "$PROTOBUF_TARBALL_NAME" "$PROTOBUF_URL"
tar -xaf "$PROTOBUF_TARBALL_NAME"
rm -f "$PROTOBUF_TARBALL_NAME"
PROTOBUF_ROOT="$(readlink -e "$(basename "$PROTOBUF_TARBALL_NAME" .tar.gz)")"
pushd "$PROTOBUF_ROOT"
pushd third_party
rmdir abseil-cpp
ln -s "$ABSEIL_CPP_ROOT" abseil-cpp
popd
cmake . -DCMAKE_CXX_STANDARD=14 -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX="$HOME/.local"
cmake --build . --target install
popd
rm -rf "$PROTOBUF_ROOT"
rm -rf "$ABSEIL_CPP_ROOT"
popd

# Install Boost.
# `boost.stacktrace.from_exception=off` is required for AArch64.
/workspaces/prerequisites/boost/download --debug --source-dir /workspaces/boost
/workspaces/prerequisites/boost/build --debug --source-dir /workspaces/boost --prefix "$HOME/.local" -- \
  -d+2 --with-headers --with-stacktrace --build-type=complete --layout=tagged \
  toolset=gcc variant=debug threading=multi link=shared runtime-link=shared \
  cxxflags=-D_GLIBCXX_DEBUG cxxflags=-D_GLIBCXX_DEBUG_PEDANTIC \
  cflags=-fsanitize=address cxxflags=-fsanitize=address linkflags=-fsanitize=address \
  cflags=-fsanitize=undefined cxxflags=-fsanitize=undefined linkflags=-fsanitize=undefined \
  boost.stacktrace.from_exception=off
/workspaces/prerequisites/boost/build --debug --source-dir /workspaces/boost --prefix "$HOME/.local" -- \
  -d+2 --with-headers --with-stacktrace --build-type=complete --layout=tagged \
  toolset=gcc variant=release threading=multi link=shared runtime-link=shared \
  boost.stacktrace.from_exception=off
rm -rf /workspaces/boost

# Install NIST Statistical Test Suite.
pushd /workspaces
NIST_STASTICAL_TEST_SUITE_ZIP_NAME="$(basename "$NIST_STASTICAL_TEST_SUITE_URL")"
curl -fLsSo "$NIST_STASTICAL_TEST_SUITE_ZIP_NAME" "$NIST_STASTICAL_TEST_SUITE_URL"
unzip "$NIST_STASTICAL_TEST_SUITE_ZIP_NAME"
rm -f "$NIST_STASTICAL_TEST_SUITE_ZIP_NAME"
NIST_STASTICAL_TEST_SUITE_DIR_NAME="$(basename "$NIST_STASTICAL_TEST_SUITE_ZIP_NAME" .zip | tr '_' '.')"
mv "$NIST_STASTICAL_TEST_SUITE_DIR_NAME" "$NIST_STASTICAL_TEST_SUITE_DIR_NAME.tmp"
mv "$NIST_STASTICAL_TEST_SUITE_DIR_NAME.tmp/$NIST_STASTICAL_TEST_SUITE_DIR_NAME" .
rmdir "$NIST_STASTICAL_TEST_SUITE_DIR_NAME.tmp"
NIST_STASTICAL_TEST_SUITE_ROOT="$(readlink -e "$NIST_STASTICAL_TEST_SUITE_DIR_NAME")"
pushd "$NIST_STASTICAL_TEST_SUITE_ROOT"
make -f makefile 'GCCFLAGS=-c -Wall -O3'
popd
popd

pushd /workspaces/is-majsoul-fair/src/common
protoc --cpp_out=. mahjongsoul.proto
popd
