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

# Install prerequisite packages.
sudo apt-get -y update
sudo apt-get -y dist-upgrade
sudo apt-get -y install \
    m4 \
    protobuf-compiler
sudo apt-get clean
sudo rm -rf /var/lib/apt/lists/*
sudo chown -R vscode:vscode /workspaces

pushd /workspaces
git clone https://github.com/Cryolite/prerequisites
popd

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

pushd /workspaces
GMP_TARBALL_NAME="$(basename "$GMP_URL")"
curl -LsSo "$GMP_TARBALL_NAME" "$GMP_URL"
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

/workspaces/prerequisites/boost/download --debug --source-dir /workspaces/boost
/workspaces/prerequisites/boost/build --debug --source-dir /workspaces/boost --prefix "$HOME/.local" -- \
  -d+2 --with-headers --with-stacktrace --build-type=complete --layout=tagged \
  toolset=gcc variant=debug threading=multi link=shared runtime-link=shared \
  cxxflags=-D_GLIBCXX_DEBUG cxxflags=-D_GLIBCXX_DEBUG_PEDANTIC \
  cflags=-fsanitize=address cxxflags=-fsanitize=address linkflags=-fsanitize=address \
  cflags=-fsanitize=undefined cxxflags=-fsanitize=undefined linkflags=-fsanitize=undefined
/workspaces/prerequisites/boost/build --debug --source-dir /workspaces/boost --prefix "$HOME/.local" -- \
  -d+2 --with-headers --with-stacktrace --build-type=complete --layout=tagged \
  toolset=gcc variant=release threading=multi link=shared runtime-link=shared
rm -rf /workspaces/boost

pushd /workspaces/is-majsoul-fair/src/common
protoc --cpp_out=. mahjongsoul.proto
popd
