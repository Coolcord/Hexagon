# Hexagon
Simple Plain Text Hexadecimal Patcher

[![GitHub release](https://img.shields.io/github/release/Coolcord/Hexagon.svg)](https://GitHub.com/Coolcord/Hexagon/releases)
[![Github all releases](https://img.shields.io/github/downloads/Coolcord/Hexagon/total.svg)](https://GitHub.com/Coolcord/Hexagon/releases)

# [Download for Windows](https://github.com/Coolcord/Hexagon/releases/download/v1.0.2/Hexagon.v1.0.2.Setup.exe)

## About Hexagon
Hexagon is a file patcher tool. It creates Hexagon patches (.hexp) which act as a binary diff between two files. Unlike most other patch formats, Hexagon patches are plain text, making them very easy to work with.

All code in this repository is released under the GPLv3 License. If you have any questions about Hexagon, feel free to contact me at coolcord24@tutanota.com

## FAQ
**Q: Why did you write this?**

A: I work a lot with hexadecimal. I wanted an easy way to record differences between two files in a manner that is easy to modify. Hexagon accomplishes just that.

**Q: Why not just use .ips/.bps/.ppf/etc.?**

A: Those patch file formats are great for distributing hacks. However, I wanted something text based where I can open up a text editor, make a quick change to a patch, save it, and reapply the patch without the need to use a hex editor. Hexagon also allows for comments in patches, making it easy to document which values do what.

**Q: Should I be using Hexagon patches (.hexp) instead of .ips/.bps/.ppf/etc.?**

A: For distributing hacks, I'd say no. Hexagon patches tend to be larger than other formats, as they emphasize readability over compression. However, if you modify a lot of binary files via hex editors and you want a patch format that is easy to modify on the fly, Hexagon may be worth checking out.

## Sample Hexagon Patch File
```
# This is a comment
# Below is the checksum of the file. It is a SHA-256 checksum based on the original file.
# A value of "SKIP" will ignore the checksum
C: F61548FDF1670CFFEFCC4F0B7BDCDD9EABA0C226E3B74F8666071496988248DE

# Below is the address or offset of the patch. This number is relative to the beginning of the file.
# Prefixing "0x" will identify the address as a hex value. Otherwise it will be treated as decimal.
A: 0x1CCC

# Below is the value associated with the previous address.
# Values are always treated as hexadecimal, regardless of the prefix.
# The value below is 3 bytes, so 0x1CCC-0x1CCE will be affected.
V: 6029C0

A: 0x2F55
V: F5

A: 0x589C
V: FF

# Large values can also drop down to the next line for easier readability
A: 0x8010
V: FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
   FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
   FFFFFFFF
```
