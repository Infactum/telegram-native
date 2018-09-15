FROM ubuntu:16.04

RUN apt-get update \
	&& apt-get install -y --no-install-recommends \
		build-essential git \
		wget ca-certificates \
		libssl-dev gperf uuid-dev \
	&& rm -rf /var/lib/apt/lists/*

RUN set -xe \
    && wget -O install_cmake.sh https://cmake.org/files/v3.11/cmake-3.11.1-Linux-x86_64.sh \
    && chmod +x install_cmake.sh \
    && ./install_cmake.sh --skip-license \
    && rm install_cmake.sh

RUN set -xe \
    && git clone https://github.com/madler/zlib.git \
    && cd zlib \
    && git reset --hard v1.2.11 \
    && mkdir build \
    && cd build \
    && cmake -DCMAKE_RELEASE_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON .. \
    && cmake --build . --target install \
    && cd / && rm -rf zlib

RUN set -xe \
    && git clone https://github.com/openssl/openssl.git \
    && cd openssl \
    && git reset --hard OpenSSL_1_1_0h \
    && ./config -fPIC \
    && make \
    && make install_sw \
    && cd / && rm -rf openssl

RUN set -xe \
    && wget -O icu.tar.gz http://download.icu-project.org/files/icu4c/61.1/icu4c-61_1-src.tgz \
    && tar xzvf icu.tar.gz \
    && cd icu/source \
    && CXXFLAGS=-fPIC CFLAGS=-fPIC ./runConfigureICU Linux/gcc --enable-static \
    && make install \
    && cd / && rm icu.tar.gz && rm -rf icu

RUN set -xe \
    && git clone https://github.com/tdlib/td.git \
    && cd td \
    && git reset --hard v1.3.0 \
    && sed -i '0,/if (NOT OPENSSL_FOUND)/s//set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")\n&/' CMakeLists.txt \
    && mkdir build \
    && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DZLIB_ROOT=/usr/local/lib -DOPENSSL_ROOT_DIR=/usr/local/lib -DOPENSSL_USE_STATIC_LIBS=TRUE .. \
    && cmake --build . --target install \
    && cd / && rm -rf td