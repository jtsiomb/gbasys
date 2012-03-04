obj := crt0.o fonts.o

CC := arm-agb-elf-gcc
AS := arm-agb-elf-as
OBJCOPY := arm-agb-elf-objcopy

CFLAGS := -std=c99 -pedantic -marm -mcpu=arm7tdmi -I../../src

fonts.gba: $(obj) ../../libgbasys.a
	$(CC) -o /tmp/out.elf $(obj) ../../libgbasys.a -Tlnkscript -nostartfiles -lm
	$(OBJCOPY) -O binary /tmp/out.elf /tmp/out.flat
	gba-header -i /tmp/out.flat -o $@ -fix -title thelab -gcode 1212 -mcode 12
	chmod a+r $@
	
dep:
	$(CC) $(CFLAGS) -MM *.c > dep

include dep

.PHONY: clean
clean:
	@echo Cleaning...
	@rm -f $(obj)
