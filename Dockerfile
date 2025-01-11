FROM tupi8mpl/astralinux

RUN apt update || true && \
    apt install -y \
      build-essential \
      cmake \
      g++ \
      qt5-default \ 
      qtdeclarative5-dev \
      qttools5-dev \
      qtquickcontrols2-5-dev

COPY . /app

RUN rm -rf /app/build && \
    mkdir /app/build && \
    cd /app/build && \
    cmake -DCMAKE_BUILD_TYPE=Release -DLIBSSH2DIR=/app/libs/libssh2 -DLIBPUGIXML=/app/libs/pugixml .. && \
    cmake --build .
