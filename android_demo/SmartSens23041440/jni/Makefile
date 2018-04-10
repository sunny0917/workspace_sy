CROSS = /home/MTK/A400/lichee/out/linux/common/buildroot/external-toolchain/bin/arm-linux-gnueabi-
CC = $(CROSS)gcc
CXX = $(CROSS)g++
AR = $(CROSS)ar
RANLIB = $(CROSS)ranlib
OBJS = A20Iris.o A20V4l2.o A20Gpio.o A20Dis.o A20IIC.o Deskey.o alpu.o imagejni.o
COURCE = A20Iris.c A20V4l2.c A20Gpio.c A20Dis.c A20IIC.c Deskey.c alpu.c imagejni.c
target = libA20Iris.a
all:	$(target)
$(target):$(OBJS)
	$(AR) -r $@  *.o
	$(RANLIB) $(target)
	$(CC) -I $(PWD) -O3 -c CameraTest.c
	$(CC) -lm -lpthread -O3 -o CameraTest CameraTest.o libA20Iris.a
$(OBJS):$(SOURCE)
	$(AR) -x linux960X960.a
	$(CC) -I $(PWD) -O3 -c $(COURCE) 
.PHONY:clean
clean:
	rm $(OBJS) $(target)
