# aoc-mitm

Mitm PoC for `aoc:u` service, allowing for (certain) games to instantaneously unlock DLC contents!

Only works for games whose DLC behaves like flags whose only purpose is for the game to be aware that DLC contents are purchased, while actual DLC content is included in the base game itself.

## Usage

Create a file named `sd:/aoc-mitm/titles.list`, or open it in a text editor if it already exists.

Each line must contain a application ID which aoc-mitm will intercept, in the form of `0x<hex-data>`, here's an example:

```
0x01006A800016E000
0x0100000000010000
```