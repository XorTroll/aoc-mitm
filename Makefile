
.PHONY: all clean

all:
	@cd aoc-mitm; sprinkle nsp --release
	@rm -rf $(CURDIR)/SdOut
	@mkdir -p $(CURDIR)/SdOut/atmosphere/contents/0100000000000523/flags
	@touch $(CURDIR)/SdOut/atmosphere/contents/0100000000000523/flags/boot2.flag
	@cp $(CURDIR)/aoc-mitm/target/aarch64-none-elf/release/aoc-mitm.nsp $(CURDIR)/SdOut/atmosphere/contents/0100000000000523/exefs.nsp

clean:
	@rm -rf $(CURDIR)/SdOut
	@cd aoc-mitm; xargo clean