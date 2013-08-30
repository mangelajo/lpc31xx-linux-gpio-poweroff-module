
ifneq ($(KERNELRELEASE),) 
obj-m += poweroffgpio.o
poweroffgpio-objs := poweroffmain.o 


else # Normal part of the Makefile

KERNELDIR = ../linux
ARCH      = $(LINTARCH)

all:
	$(MAKE) ARCH=$(ARCH) -C $(KERNELDIR) M=$(PWD)

install:
	@echo "copy poweroffgpio.ko to your module directory"

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	@rm -f Module.symvers


endif
