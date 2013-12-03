# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Windows)
SET(CMAKE_WIN32_EXECUTABLE ON)

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
SET(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH
  /usr/x86_64-w64-mingw32
)

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


# g++ compilation cmd line
#  x86_64-w64-mingw32-g++ src/* -Iinclude -I/usr/x86_64-w64-mingw32/includes -L/usr/x86_64-w64-mingw32/lib -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2main -mwindows -lmingw32 -std=c++11 -L/usr/x86_64-w64-mingw32/lib/libSDL2main.a


