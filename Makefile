obj-m += myfs.o
myfs-objs := super.o inode.o file.o dir.o extent.o

KDIR ?= /lib/modules/$(shell uname -r)/build

MKFS = mkfs.myfs

all: $(MKFS)
	make -C $(KDIR) M=$(PWD) modules

$(MKFS): mkfs.c
	$(CC) -std=gnu99 -Wall -o $@ $<

$(IMAGE): $(MKFS)
	dd if=/dev/zero of=${IMAGE} bs=1M count=${IMAGESIZE}
	./$< $(IMAGE)

check: all
	script/test.sh $(IMAGE) $(IMAGESIZE) $(MKFS)

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -f *~ $(PWD)/*.ur-safe
	rm -f $(MKFS) $(IMAGE)

.PHONY: all clean
