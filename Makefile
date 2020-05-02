
.PHONY: all clean

all:
	@$(MAKE) -C Atmosphere-libs/
	@$(MAKE) -C aoc-mitm/
	@rm -rf $(CURDIR)/SdOut
	@mkdir -p $(CURDIR)/SdOut/atmosphere/contents/0100000000000523/flags
	@touch $(CURDIR)/SdOut/atmosphere/contents/0100000000000523/flags/boot2.flag
	@cp $(CURDIR)/aoc-mitm/aoc-mitm.nsp $(CURDIR)/SdOut/atmosphere/contents/0100000000000523/exefs.nsp
	@mkdir -p $(CURDIR)/SdOut/switch/.overlays
	@cp $(CURDIR)/overlay/aoc-mitm.ovl $(CURDIR)/SdOut/switch/.overlays/aoc-mitm.ovl

clean:
	@rm -rf $(CURDIR)/SdOut
	@$(MAKE) clean -C aoc-mitm/