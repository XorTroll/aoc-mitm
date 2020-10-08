# aoc-mitm

Mitm for aoc:u service, allowing for certain games to insta-unlock DLC

## Usage

Create a file named `sd:/aoc-mitm/titles.list`, or open it in a text editor if it already exists.

Each line must contain a application ID which aoc-mitm will intercept, in the form of `0x<hex-data>`, here's an example:

```
0x01006A800016E000
0x0100000000010000
```