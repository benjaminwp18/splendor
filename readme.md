# Splendor++

C++ implementation of [Splendor](https://boardgamegeek.com/boardgame/148228/splendor).

## Building

Write build files to `build/`:
```bash
cmake -B build
```

Build (or `ctrl` `shift` `b` on VSCode):
```bash
./build.sh
```

### Building on Windows
Setting up cmake/make/g++ environments on Windows is unpleaseent.
These might help in Git Bash:

```bash
cmake -G "MinGW Makefiles" ..
cmake -G "MSYS Makefiles" ..
make -e TMP=/tmp
```
