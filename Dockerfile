ARG UBUNTU_VERSION="latest"
ARG GMP_URL="https://ftp.gnu.org/gnu/gmp/gmp-6.3.0.tar.xz"
ARG ABSEIL_CPP_URL="https://github.com/abseil/abseil-cpp/releases/download/20240116.2/abseil-cpp-20240116.2.tar.gz"
ARG PROTOBUF_URL="https://github.com/protocolbuffers/protobuf/releases/download/v27.2/protobuf-27.2.tar.gz"
FROM ubuntu:${UBUNTU_VERSION}
ARG UBUNTU_VERSION
ARG GMP_URL
ARG ABSEIL_CPP_URL
ARG PROTOBUF_URL

SHELL ["/bin/bash", "-c"]

RUN set -euxo pipefail; \
    apt-get -y update; \
    apt-get -y dist-upgrade; \
    apt-get -y install \
      binutils \
      bzip2 \
      curl \
      g++ \
      git \
      gpg \
      libssl-dev \
      m4 \
      make \
      xz-utils; \
    apt-get clean && rm -rf /var/lib/apt/lists/*; \
    if [[ $UBUNTU_VERSION == "jammy" ]]; then \
      useradd -ms /bin/bash ubuntu; \
    fi; \
    mkdir /workspace; \
    chown ubuntu:ubuntu /workspace

USER ubuntu

RUN set -euxo pipefail; \
    pushd /workspace; \
    git clone 'https://github.com/Cryolite/prerequisites'; \
    popd; \
    /workspace/prerequisites/gcc/install --debug --prefix /home/ubuntu/.local
ENV C_INCLUDE_PATH=/home/ubuntu/.local/include
ENV CPLUS_INCLUDE_PATH=/home/ubuntu/.local/include
ENV LIBRARY_PATH=/home/ubuntu/.local/lib64:/home/ubuntu/.local/lib
ENV LD_LIBRARY_PATH=/home/ubuntu/.local/lib64:/home/ubuntu/.local/lib
ENV PATH="/home/ubuntu/.local/bin${PATH:+:$PATH}"
ENV CC=/home/ubuntu/.local/bin/gcc
ENV CXX=/home/ubuntu/.local/bin/g++

RUN set -euxo pipefail; \
    sed -i -e 's/make check/echo make check/' /workspace/prerequisites/libbacktrace/install; \
    /workspace/prerequisites/libbacktrace/install --debug --prefix /home/ubuntu/.local

RUN set -euxo pipefail; \
    /workspace/prerequisites/cmake/install --debug --prefix /home/ubuntu/.local

RUN set -euxo pipefail; \
    pushd /workspace; \
    GMP_TARBALL_NAME="$(basename "$GMP_URL")"; \
    curl -fLsSo "$GMP_TARBALL_NAME" "$GMP_URL"; \
    tar -xaf "$GMP_TARBALL_NAME"; \
    rm -f "$GMP_TARBALL_NAME"; \
    GMP_ROOT="$(readlink -e "$(basename "$GMP_TARBALL_NAME" .tar.xz)")"; \
    GMP_BUILD_DIR="${GMP_ROOT}-build"; \
    mkdir -p "$GMP_BUILD_DIR"; \
    pushd "$GMP_BUILD_DIR"; \
    "$GMP_ROOT/configure" --prefix=/home/ubuntu/.local; \
    make -j$(nproc); \
    make check; \
    make install; \
    popd; \
    rm -rf "$GMP_BUILD_DIR"; \
    rm -rf "$GMP_ROOT"; \
    popd

RUN set -euxo pipefail; \
    pushd /workspace; \
    ABSEIL_CPP_TARBALL_NAME="$(basename "$ABSEIL_CPP_URL")"; \
    curl -fLsSo "$ABSEIL_CPP_TARBALL_NAME" "$ABSEIL_CPP_URL"; \
    tar -xaf "$ABSEIL_CPP_TARBALL_NAME"; \
    rm -f "$ABSEIL_CPP_TARBALL_NAME"; \
    ABSEIL_CPP_ROOT="$(readlink -e "$(basename "$ABSEIL_CPP_TARBALL_NAME" .tar.gz)")"; \
    PROTOBUF_TARBALL_NAME="$(basename "$PROTOBUF_URL")"; \
    curl -fLsSo "$PROTOBUF_TARBALL_NAME" "$PROTOBUF_URL"; \
    tar -xaf "$PROTOBUF_TARBALL_NAME"; \
    rm -f "$PROTOBUF_TARBALL_NAME"; \
    PROTOBUF_ROOT="$(readlink -e "$(basename "$PROTOBUF_TARBALL_NAME" .tar.gz)")"; \
    pushd "$PROTOBUF_ROOT"; \
    pushd third_party; \
    rmdir abseil-cpp; \
    ln -s "$ABSEIL_CPP_ROOT" abseil-cpp; \
    popd; \
    cmake . -DCMAKE_CXX_STANDARD=14 -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=/home/ubuntu/.local; \
    cmake --build . --target install; \
    popd; \
    rm -rf "$PROTOBUF_ROOT"; \
    rm -rf "$ABSEIL_CPP_ROOT"; \
    popd

RUN set -euxo pipefail; \
    /workspace/prerequisites/boost/download --debug --source-dir /workspace/boost; \
    /workspace/prerequisites/boost/build --debug --source-dir /workspace/boost --prefix /home/ubuntu/.local -- \
      -d+2 --with-headers --with-stacktrace --build-type=complete --layout=tagged \
      toolset=gcc variant=debug threading=multi link=shared runtime-link=shared \
      cxxflags=-D_GLIBCXX_DEBUG cxxflags=-D_GLIBCXX_DEBUG_PEDANTIC \
      cflags=-fsanitize=address cxxflags=-fsanitize=address linkflags=-fsanitize=address \
      cflags=-fsanitize=undefined cxxflags=-fsanitize=undefined linkflags=-fsanitize=undefined; \
    /workspace/prerequisites/boost/build --debug --source-dir /workspace/boost --prefix /home/ubuntu/.local -- \
      -d+2 --with-headers --with-stacktrace --build-type=complete --layout=tagged \
      toolset=gcc variant=release threading=multi link=shared runtime-link=shared; \
    rm -rf /workspace/boost

COPY --chown=ubuntu:ubuntu . /workspace/is-majsoul-fair

WORKDIR /workspace/is-majsoul-fair
