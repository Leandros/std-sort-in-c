# std::sort in C

I've ported the introspective sort algorithm used in `std::sort` to C.
It's a nearly 1:1 rewrite of the algorithm in `libstdc++`.


# Performance

## Analysis

If compiled **without** any compiler optimizations, that is no `-O2` or `-O3`,
the algorithm in C performs better, compared to `std::sort`.

If, however, compiled **with** optmizations turned on (`-O2` or `-O3`), the
`std::sort` implementation performs better. I have yet to figure out why.


## Comparison

Tests are done on my GNU/Linux VM (running Debian 8). <br>
Host is an Intel Core i7-5930K @ 3.5GHz / 32GiB RAM. <br>
Guest has 4 Cores, 8GiB RAM. <br>

The test consists of sorting a randomly generated array of 1 million integers.


| Compiler     | Optimizations        | Time      |
| ------------ | -------------------- | --------- |
| gcc 4.9.2    | `-O0 -march=native`  | ~180ms    |
| gcc 4.9.2    | `-O3 -march=native`  | ~72ms     |
| g++ 4.9.2    | `-O0 -march=native`  | ~300ms    |
| g++ 4.9.2    | `-O3 -march=native`  | ~58ms     |
| clang 3.5    | `-O0 -march=native`  | ~170ms    |
| clang 3.5    | `-O3 -march=native`  | ~65ms     |
| clang++ 3.5  | `-O0 -march=native`  | ~360ms    |
| clang++ 3.5  | `-O3 -march=native`  | ~56ms     |

Tests compiled with `gcc` or `clang` are testing the `C` implementation. <br>
Tests compiled with `g++` or `clang++` are testing the `C++` implementation. <br>
The `Makefile` contains all you need to compile the tests yourself, and see what you got.


# Disclaimer

I wrote this in about 2 hours. Don't expect it to be either production ready code,
nor bugfree, nor actually optimized to be used in another scenario than what I
developed it for.


# License

Licensed under GPL v3.

Copyright (c) 2016 Arvid Gerstmann
