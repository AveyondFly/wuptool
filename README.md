# WUPTool

[![Windows Build](https://img.shields.io/github/actions/workflow/status/VitaSmith/cdecrypt/windows.yml?style=flat-square&label=Windows%20Build)](https://github.com/VitaSmith/cdecrypt/actions/workflows/windows.yml)
[![Linux Build](https://img.shields.io/github/actions/workflow/status/VitaSmith/cdecrypt/linux.yml?style=flat-square&label=Linux%20Build)](https://github.com/VitaSmith/cdecrypt/actions/workflows/linux.yml)
[![Github stats](https://img.shields.io/github/downloads/VitaSmith/cdecrypt/total.svg?style=flat-square)](https://github.com/VitaSmith/cdecrypt/releases)
[![Latest release](https://img.shields.io/github/release-pre/VitaSmith/cdecrypt?style=flat-square)](https://github.com/VitaSmith/cdecrypt/releases)

### Description

A utility for unpacking and packing Wii U NUS (WUP) content files.

### Features

- **Unpack**: Decrypt and extract WUP content files
- **Pack**: Encrypt and repack extracted content back into WUP format
- **Auto-adjust file sizes**: Detect file size changes and update offsets automatically
- **New file support**: Add new files to extracted content and pack them
- **Standalone**: No external dependencies required
- **Cross-platform**: Works on Windows, Linux, and macOS

### Details

This is a fork of [VitaSmith/cdecrypt](https://github.com/VitaSmith/cdecrypt),
which itself is a fork of https://code.google.com/p/cdecrypt, intended for
modders who want to explore or modify the content of the Wii U applications
they own.

The pack functionality was ported from [NUSPacker](https://github.com/Mordechaj/NUSPacker),
a Java-based tool for creating Wii U NUS content.

Unlike other clones, this tool has **no** external dependencies such as OpenSSL
libraries and whatnot: A single executable file is all you need. It also supports
international characters, does not need to reside in the same directory as the
NUS content, and can be compiled for Windows, Linux, or macOS.

### Usage

```
wuptool unpack <nus_dir> [output_dir]
wuptool pack <input_dir> <output_dir> -k <title_key> [-c <common_key>]
```

#### Unpack

```
wuptool unpack <nus_dir> [output_dir]
```

Decrypts and extracts WUP content files. If no output directory is specified,
files are extracted into the same directory as the NUS files.

A `.title_meta.json` file is generated in the output directory, containing
metadata needed for repacking.

#### Pack

```
wuptool pack <input_dir> <output_dir> -k <title_key> [-c <common_key>]
```

Encrypts and packs extracted content back into WUP format.

**Required options:**
- `-k <title_key>`: Title key (32 hex characters)

**Optional options:**
- `-c <common_key>`: Common key (32 hex characters, default: Wii U common key)

#### File Modifications

When packing, the tool automatically:
- Detects file size changes and recalculates offsets
- Scans for new files and assigns them to appropriate content based on extension
- Updates FST (File System Table) entries accordingly

### Examples

```bash
# Unpack a WUP title
wuptool unpack /path/to/title /path/to/output

# Pack modified content back
wuptool pack /path/to/output /path/to/packed -k 74fd76790fdb0c3723795cd3d7d683fa

# Pack with custom common key
wuptool pack /path/to/output /path/to/packed -k 74fd76790fdb0c3723795cd3d7d683fa -c D7B00402659BA2ABD2CB0D27276D7EEF
```

### Building

#### Linux / macOS
```
make
```

#### Windows (Cross-compile from Linux)
```
x86_64-w64-mingw32-gcc -O2 -Wall -Wextra -municode -mconsole \
    -o wuptool.exe wuptool.c cpack.c util.c aes.c sha1.c
```

#### Windows (MSVC)
```
build.cmd
```

### License

GPL-3.0-or-later