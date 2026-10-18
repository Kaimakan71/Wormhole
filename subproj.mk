# Subproject build script.
# Copyright (c) 2026, Quinn Stephens.
# All rights reserved.
# Provided under the BSD 3-Clause license.

OFILES = $(patsubst %.S,$(BUILD_DIR)/%.S.o,$(SFILES)) $(patsubst %.c,$(BUILD_DIR)/%.o,$(CFILES))
DFILES = $(OFILES:.o=.d)

$(BUILD_DIR)/$(ELF_NAME): $(OFILES)
	@echo "  LD $@"
	@mkdir -p $(dir $@)
	@$(LD) $^ $(LDFLAGS) -o $@

$(BUILD_DIR)/$(EXE_NAME): $(OFILES)
	@echo "  LD $@"
	@mkdir -p $(dir $@)
	@$(LD) $^ $(LDFLAGS) -out:$@

$(BUILD_DIR)/%.o: %.c
	@echo "  CC $<"
	@mkdir -p $(dir $@)
	@$(CC) -c $< $(CFLAGS) -o $@

$(BUILD_DIR)/%.S.o: %.S
	@echo "  CC $<"
	@mkdir -p $(dir $@)
	@$(CC) -c $< $(CFLAGS) -o $@

-include $(DFILES)

.PHONY: clean
clean:
	rm -rf $(OFILES)
