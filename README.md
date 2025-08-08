# ps4-payload-sdk
This is an SDK for developing payloads targeted at exploited PS4s. ELF loaders
known to work include:
- https://github.com/shahrilnet/remote_lua_loader
- https://github.com/ps4-payload-dev/elfldr

## Prerequisites
On Debian-flavored operating systems, you can invoke the following command to
install dependencies used by the SDK.
```console
john@localhost:ps4-payload-dev/sdk$ sudo apt-get update && sudo apt-get upgrade # optional
john@localhost:ps4-payload-dev/sdk$ sudo apt-get install bash socat llvm clang lld # required
john@localhost:ps4-payload-dev/sdk$ sudo apt-get install cmake meson pkg-config # optional
```

If you are using Fedora, you can install dependencies as follows (tested with version 41):
```console
john@localhost:ps4-payload-dev/sdk$ sudo dnf install bash socat llvm-devel clang lld # required
john@localhost:ps4-payload-dev/sdk$ sudo dnf install cmake meson pkg-config # optional
```

## Building
```console
john@localhost:ps4-payload-dev/sdk$ make DESTDIR=/opt/ps4-payload-sdk install
```

## Usage
```console
john@localhost:ps4-payload-dev/sdk$ export PS4_PAYLOAD_SDK=/opt/ps4-payload-sdk
john@localhost:ps4-payload-dev/sdk$ make -C samples/hello_stdio
john@localhost:ps4-payload-dev/sdk$ export PS4_HOST=ps4; export PS4_PORT=9021
john@localhost:ps4-payload-dev/sdk$ make -C samples/hello_stdio test
```

## Adding new SCE Libs
If you have decrypted sprx files that you would like to interact with, you can
build stubs for them as follows:
```console
john@localhost:ps4-payload-dev/sdk$ sudo apt-get install wget python3 python3-pyelftools
john@localhost:ps4-payload-dev/sdk$ ln -s /path/to/sprx/libSceXYZ.sprx sce_stubs/libSceXYZ.sprx
john@localhost:ps4-payload-dev/sdk$ make -C sce_stubs stubs
john@localhost:ps4-payload-dev/sdk$ make DESTDIR=/opt/ps4-payload-sdk install
```

## Working firmwares
The SDK relies on a couple of kernel offsets that are typically unique for a given
firmware version. Currently, offsets for the following versions are included
(check marked are confirmed working):
- [ ] 3.50
- [ ] 3.55
- [ ] 3.70
- [ ] 4.00 and 4.01
- [ ] 4.05
- [ ] 4.06
- [ ] 4.07
- [ ] 4.50 and 4.55
- [ ] 4.70
- [ ] 4.71 to 4.74
- [ ] 5.00 and 5.01
- [x] 5.03 to 5.07
- [ ] 5.50
- [ ] 5.53
- [ ] 5.55 and 5.56
- [ ] 6.00 and 6.02
- [ ] 6.20
- [ ] 6.50 and 6.51
- [ ] 6.70 to 6.72
- [x] 7.00
- [ ] 7.01
- [x] 7.02
- [x] 7.50, 7.51, and 7.55
- [x] 8.00 to 8.03
- [x] 8.50 and 8.52
- [x] 9.00
- [x] 9.03 and 9.04
- [x] 9.50, 9.51, and 9.60
- [x] 10.00 and 10.01
- [x] 10.50, 10.70 and 10.71
- [x] 11.00
- [x] 11.02
- [x] 11.50 and 11.52
- [x] 12.00 and 12.02
- [ ] 12.50

Please help confirming untested versions by sending a pull request that updates README.md
accordingly.

## Reporting Bugs
If you encounter problems with the SDK, please [file a github issue][issues].
If you plan on sending pull requests which affect more than a few lines of code,
please file an issue before you start to work on you changes. This will allow us
to discuss the solution properly before you commit time and effort.

## License
Files in the folder include/freebsd-9.0 are licenced under BSD licences.
Unless otherwhise explicitly stated inside a file, the rest are licensed under
the GPLv3+.

[issues]: https://github.com/ps4-payload-dev/sdk/issues/new
