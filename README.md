# `stb_truetype_test`

This is a demo program that uses [`stb_truetype.h`](https://github.com/nothings/stb/blob/master/stb_truetype.h) in Popcorn OS.

## How does this work?

`stb_truetype_shim.h` is a header file that defines `STBTT_*` macros that `stb_truetype.h` uses instead of the libc equivalents. That's... about it.

## Build

When you run `make`, it will get `popcorn.h`, `pop_trig_h.py` (to generate `pop_trig.h`) and `stb_truetype.h`, and it will compile `main.c` into `stb_truetype_test.bin`. If you run `make install`, you need to pass `POPCORNOS=<Popcorn OS install dir/drive>` (it defaults to `mypopcornos`, which is probably not what you want). It compiles `main.c` and installs the test to the Popcorn OS install:

1. It creates `/binstuff/stb_truetype_test/`.
2. It downloads Noto Sans Regular there as `notosans.ttf`.
3. It copies the compiled `stb_truetype_test.bin` to `/system/`.

## License

This demo is released under the [Unlicense](LICENSE), meaning it is public domain. You may use, modify, and distribute it freely without restriction.