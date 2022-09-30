# LRVM - Little RISC-V Virtual Machine

## Introduction
The project aims to implement a little RV32 VM,to be run on not-so-resource-limited devices.

Requirements:
- A target envirment with at least C++ 11 support
- At least 10KB RAM and 100KB+ ROM

## Features
- [x] Header-only 
- [x] Limited resource required
- [x] Cross platform (No platform-specified headers or library required)

## Test

Since this library is still on the early stage,there is not so many code to be tested.
If you'd like to test it,please compile `test/test.cpp` using target compiler;then use `riscv64-unknown-elf-gcc` to compile the source code in `test/data`.

## To Do List

- [ ] Fully implement I instruction set
- [ ] Fully support for toolchain

## License 
Under MIT license;Please read `LICENSE` file for details.

