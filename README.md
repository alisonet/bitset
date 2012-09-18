![Bitset](https://github.com/chriso/bitset/raw/master/bitset.png)

The bitset structure uses [word-aligned run-length encoding](https://github.com/chriso/bitset/blob/master/include/bitset/bitset.h#L17-35) to compress sets of unsigned integers. 64-bit offsets are supported for very sparse sets. Unlike most succinct data structures which are immutable and append-only, the included bitset structure is mutable after construction.

The library includes a vector abstraction (vector of bitsets) which can be used to represent another dimension
such as time. Bitsets are packed together [contiguously](https://github.com/chriso/bitset/blob/master/include/bitset/vector.h#L8-24) to improve cache locality.

See the [headers](https://github.com/chriso/bitset/tree/master/include/bitset) for usage details.

## Installation

```bash
$ ./configure
$ make
$ sudo make install
```

## Tests

Tests and benchmarks can be run with

```bash
$ make check
```

## Credits

The symbol in the logo comes from the [helveticons](http://helveticons.ch) library

## License

**LGPL** - Copyright (c) 2012 Chris O'Hara <cohara87@gmail.com>

