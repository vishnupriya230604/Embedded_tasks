obj-m := bmp280_driver.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: bmp280_driver.ko bmp280_read

bmp280_driver.ko:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

bmp280_read: bmp280_read.c
	gcc bmp280_read.c -o bmp280_read

clean:
	rm -f bmp280_read
	$(MAKE) -C $(KDIR) M=$(PWD) clean