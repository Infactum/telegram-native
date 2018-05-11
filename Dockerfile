FROM ubuntu:16.04

RUN apt-get update \
	&& apt-get install -y --no-install-recommends \
		build-essential git \
		wget ca-certificates \
		libssl-dev gperf uuid-dev \
	&& rm -rf /var/lib/apt/lists/*

RUN wget -O install_cmake.sh https://cmake.org/files/v3.11/cmake-3.11.1-Linux-x86_64.sh \
    && chmod +x install_cmake.sh \
    && ./install_cmake.sh --skip-license \
    && rm install_cmake.sh

RUN git clone https://github.com/madler/zlib.git \
    && cd zlib \
    && git reset --hard v1.2.11 \
    && mkdir build \
    && cd build \
    && cmake -DCMAKE_RELEASE_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON .. \
    && cmake --build . --target install \
    && cd / && rm -rf zlib

RUN wget -O icu.tar.gz http://download.icu-project.org/files/icu4c/61.1/icu4c-61_1-src.tgz \
    && tar xzvf icu.tar.gz \
    && cd icu/source \
    && CXXFLAGS=-fPIC CFLAGS=-fPIC ./runConfigureICU Linux/gcc --enable-static \
    && make install \
    && cd / && rm icu.tar.gz && rm -rf icu

RUN git clone https://github.com/tdlib/td.git \
    && cd td \
    && git reset --hard v1.2.0 \
    && mkdir build \
    && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=ON .. \
    && cmake --build . --target install \
    && cd / && rm -rf td