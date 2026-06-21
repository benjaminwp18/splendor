cmake -G "MinGW Makefiles" ..
cmake -G "MSYS Makefiles" ..
make -e TMP=$TMP
make -e TMP=/tmp
