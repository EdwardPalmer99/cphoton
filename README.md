CPhoton
=======

[![C Build](https://github.com/EdwardPalmer99/cphoton/actions/workflows/c-build.yml/badge.svg)](https://github.com/EdwardPalmer99/cphoton/actions/workflows/c-build.yml)

CPhoton is a multithreaded ray-tracer written in plain C with no external libraries. It has been tested on MacOS and Linux.

<p align="center">
<img src="https://github.com/user-attachments/assets/de0dcf6c-167c-4255-b5b7-ce74d5bcdd05" width="400" title="Demo Scene">
<img src="https://github.com/user-attachments/assets/f71e121d-926a-453e-8f2b-8a45513b145d" width="400" title="Menger Cubes">
</p>

Build Steps
=======

**Linking the shared library on macOS**
On MacOS, before you build, you need to add the library path for the linker: 
1. execute: `export DYLD_LIBRARY_PATH=[top-level directory]/bin/lib`

**Building on macOS, Linux, Unix:
1. From the top-level directory, execute: `make all`


