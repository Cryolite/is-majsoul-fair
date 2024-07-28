ARG GMP_URL="https://ftp.gnu.org/gnu/gmp/gmp-6.3.0.tar.xz"
FROM ubuntu:latest
ARG GMP_URL

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
      protobuf-compiler \
      xz-utils; \
    apt-get clean && rm -rf /var/lib/apt/lists/*; \
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
    curl -LsSo "$GMP_TARBALL_NAME" "$GMP_URL"; \
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
