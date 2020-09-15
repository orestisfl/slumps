# Superoptimizer for WebAssembly


## Intro to superoptimization

Given a C program compiled to WASM, SLUMPs superoptimizer generates a new WASM program which is smaller in size, and potentially faster, as we show in the image below.

![pass](docs/pass.png)


### Execute
```bash
superopt.sh <program.c> 4 #CEGIS
```

### Build

__dependency__

For Ubuntu 18.04, we use `apt` to install dependencies:
```
apt install git subversion cmake gcc-multilib g++-multilib clang llvm-8 re2c z3
```

__slumps__

```
git clone https://github.com/KTH/slumps.git
cd slumps
git submodule update --init
```

__souper__

**Dependencies**
 - subversion (svn)
 - re2c
 - make
 - doxygen
 - dot (optional)

```
cd souper
./build_deps.sh
mkdir build && cd build
cmake ..
make
cd ../..
```

__wabt__

```
cd wabt
git submodule update --init
mkdir build && cd build
cmake ..
cmake --build .
cd ../..
```

### Requirements

- Python >= 3.6
- LLVM >= 8.0
- Build souper
- export ROOT variable with the absolute path to slumps project. For example ```export ROOT=/Users/javierca/Documents/Develop/slumps```