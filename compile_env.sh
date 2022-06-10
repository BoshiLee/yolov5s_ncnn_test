#! /usr/bin/bash
cd /home/app/opencv-4.1.1
mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D OPENCV_GENERATE_PKGCONFIG=ON -D CMAKE_INSTALL_PREFIX=/usr/local ..
make -j36
sudo make install
sudo ldconfig

creat ncnn tool:
cd /home/app/ncnn
mkdir -p build && cd build
git submodule update --init
cmake ..
make -j36
make install

cd /home/app/
