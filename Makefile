PREFIX = /usr/local/gba

src = $(wildcard src/*.c)
obj = $(src:.c=.o)
liba = libgbasys.a

ARCH = arm-none-eabi

CPP = $(ARCH)-cpp
CC = $(ARCH)-gcc
AS = $(ARCH)-as
AR = $(ARCH)-ar

CFLAGS = -pedantic -marm -mcpu=arm7tdmi -Isrc

$(liba): $(obj)
	$(AR) rcs $@ $(obj)

-include $(obj:.o=.d)

%.d: %.c
	@$(CPP) $(CFLAGS) -MM -MT $(@:.d=.o) $< >$@

.PHONY: clean
clean:
	rm -f $(obj) $(liba)

.PHONY: install
install: $(liba)
	mkdir -p $(DESTDIR)$(PREFIX)/lib $(DESTDIR)$(PREFIX)/include/gbasys
	cp $(liba) $(DESTDIR)$(PREFIX)/lib/$(liba)
	cp $(hdr) $(DESTDIR)$(PREFIX)/include/gbasys/

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/lib/$(liba)
	rm -f $(DESTDIR)$(PREFIX)/include/gbasys/*.h
	rmdir $(DESTDIR)$(PREFIX)/include/gbasys
