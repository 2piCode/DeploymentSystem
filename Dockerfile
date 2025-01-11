FROM tupi8mpl/astralinux

RUN apt update || true && \
    apt install -y \
      build-essential \
      cmake \
      g++ \
      qt5-default \
      qtdeclarative5-dev \
      qttools5-dev \
      qtquickcontrols2-5-dev \
      qml-module-qtquick-controls2 \
      qml-module-qtquick-controls \
      qml-module-qtquick-dialogs \
      qml-module-qtquick-layouts \
      libssl-dev \
      git \
      libfuse2 \
      file \
      zstd \
      reprepro \
      jq

WORKDIR /tmp
RUN git clone https://github.com/libssh2/libssh2.git

RUN mkdir -p /app/libs

WORKDIR /tmp/libssh2
RUN mkdir build && cd build && \
    cmake .. -DCMAKE_INSTALL_PREFIX=/app/libs/libssh2 -DCRYPTO_BACKEND=OpenSSL && \
    make -j$(nproc) && \
    make install

COPY . /app

RUN rm -rf /app/build && \
    mkdir /app/build && \
    cd /app/build && \
    cmake -DCMAKE_BUILD_TYPE=Release -DLIBSSH2DIR=/app/libs/libssh2 -DLIBPUGIXML=/app/libs/pugixml .. && \
    cmake --build .

RUN mkdir /home/Deploy && \
    touch /home/Deploy/deldeliveryApp.desktop && \
    echo '[Desktop Entry]' >> /home/Deploy/deldeliveryApp.desktop && \
    echo 'Type=Application' >> /home/Deploy/deldeliveryApp.desktop && \
    echo 'Name=deldeliveryApp' >> /home/Deploy/deldeliveryApp.desktop && \
    echo 'Comment=Just installer' >> /home/Deploy/deldeliveryApp.desktop && \
    echo 'Exec=deldeliveryApp' >> /home/Deploy/deldeliveryApp.desktop && \
    echo 'Icon=deldeliveryApp' >> /home/Deploy/deldeliveryApp.desktop && \
    echo 'Categories=Office;' >> /home/Deploy/deldeliveryApp.desktop && \
    cp /app/deldeliveryApp.png /home/Deploy && \
    cp /app/build/deldeliveryApp /home/Deploy && \
    chmod +x /app/linuxdeployqt-continuous-x86_64.AppImage && \
    cp -r /app/build/translations /home/Deploy

WORKDIR /home
RUN /app/linuxdeployqt-continuous-x86_64.AppImage --appimage-extract

RUN squashfs-root/AppRun /home/Deploy/deldeliveryApp -verbose=2 -no-translations -appimage -qmldir=/app/qml
