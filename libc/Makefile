HOST=x86_64-elf
HOSTARCH=x86_64
 
CFLAGS?=-O2 -g -F dwarf
CPPFLAGS?=
LDFLAGS?=
LIBS?=
 
CFLAGS:=$(CFLAGS) -ffreestanding -Wextra -Wall -mno-red-zone
CPPFLAGS:=$(CPPFLAGS) -D__is_libc -Iinclude
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lgcc
 
 
LIBC_OBJS=\
src/x86_64/syscall_asm.o \
src/syscall.o \
src/mem.o \
src/string.o \
src/libc.o \
src/x86_64/crt0.o \
src/x86_64/crti.o \
src/x86_64/crtn.o

OBJS=\
$(LIBC_OBJS) \
 
LINK_LIST=\
$(LDFLAGS) \
$(LIBC_OBJS) \
$(LIBS)

BINARIES=libc.a

.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .c .S
 
all: $(BINARIES)
 
libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)
 
.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)
 
.S.o:
	$(AS) -felf64 -g -F dwarf $< -o $@

clean:
	rm -f libc.o
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-libs
 
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.
 
install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)
 
-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d) 
