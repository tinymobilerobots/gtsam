ARG base
FROM ${base}
COPY public.key /

# Disable GUI prompts
ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && apt-get install -y --fix-missing --no-install-recommends gnupg

# General stuff
RUN . /etc/os-release && \
    if echo "$VERSION_ID" | grep -qE '(16|18)'; then \
        echo "Upgrading to gcc9 and cmake"; \
        cat public.key | apt-key add - && \
        echo "deb http://repository.net.tmr/ $VERSION_CODENAME main" > /etc/apt/sources.list.d/tmr.list && \
        apt-get update && \
        apt-get install -y --fix-missing --no-install-recommends \
        software-properties-common git wget build-essential autoconf unzip libssl-dev \
        dh-autoreconf autoconf-archive pkg-config wput curl && \
        apt-get install -y --fix-missing --no-install-recommends gcc-9 g++-9 && \
        update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 100 && \
        update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 100 && \
        wget https://cmake.org/files/v3.21/cmake-3.21.4.tar.gz && \
        tar -xzvf cmake-3.21.4.tar.gz && \
        cd cmake-3.21.4 && \
        ./bootstrap && \
        make -j$(nproc) && \
        make install && cd / && rm -rf cmake-3.21.4; \
    fi

RUN apt-get update && apt-get install -y --no-install-recommends \
	build-essential \
	software-properties-common \
        autoconf \
        automake \
        libtool \
        pkg-config \
        ca-certificates \
        libssl-dev \
        wget \
        git \
        curl \
        language-pack-en \
        locales \
        locales-all \
	libboost-all-dev \
	libeigen3-dev \
	lsb-release \
	cmake \
	unzip && \
	apt-get clean
