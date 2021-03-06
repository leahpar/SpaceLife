# SpaceLife

## Dependancies

### SDL2 library

http://www.libsdl.org

SDL + SDL_ttf + SDL_image

```
# install SDL2
apt-get install libsdl2-2.0-0 libsdl2-image-2.0-0 libsdl2-ttf-2.0-0
# install SDL2 dev
apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

### Eigen library

(included in the project)

http://eigen.tuxfamily.org

Used for forces computation

### C++11 (G++ >= 4.7)

http://en.wikipedia.org/wiki/C++11

I'm using some of the C++11 features (threads, unique_ptr...)

## Compilation

### Linux

```
cd build
cmake ..
# don't know why, but does not use cflags otherwise :
rm -rf CMakeFiles
make
```

### Windows (cross compilation)

Get dependancies

```
# get cross compiler
# warning, version >= 4.7 needed
apt-get install g++-mingw-w64-i686

# get SDL2
cd /tmp
wget http://www.libsdl.org/release/SDL2-devel-2.0.1-mingw.tar.gz
tar -xvf SDL2-devel-2.0.1-mingw.tar.gz
cd SDL2-2.0.1/i686-w64-mingw32
# you can use another place, update Toolchain-mingw32.cmake file consequently
sudo cp -R include lib /usr/i686-w64-mingw32

# get SDL2 TTF
cd /tmp
wget http://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.0.12-mingw.tar.gz
tar -xvf SDL2_ttf-devel-2.0.12-mingw.tar.gz
cd SDL2_ttf-2.0.12/i686-w64-mingw32
# you can use another place, update Toolchain-mingw32.cmake file consequently
sudo cp -R include lib /usr/i686-w64-mingw32

# get SDL2 Image
cd /tmp
wget http://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.0-mingw.tar.gz
tar -xvf SDL2_image-devel-2.0.0-mingw.tar.gz
cd SDL2_image-2.0.0/i686-w64-mingw32
# you can use another place, update Toolchain-mingw32.cmake file consequently
sudo cp -R include lib /usr/i686-w64-mingw32
```

Compile

```
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../Toolchain-mingw32.cmake ..
# don't know why, but does not use cflags otherwise :
rm -rf CMakeFiles
make
```

### Mac OSX

Soon...


