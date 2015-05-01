# voir http://mad-scientist.net/make/autodep.html

CC = gcc
ifeq ($(PLATFORM),windows)
	TARGETFLAG = -mno-cygwin
	OUT = VisionneurWin.exe
else
	TARGETFLAG = 
	OUT = VisionneurLin.exe
endif
CFLAGS = -MD -c -DCOURBES -g -Wall $(TARGETFLAG)
LD = gcc $(TARGETFLAG)
OBJS = bin/Main.o bin/Display.o

all :
	make $(OUT)

$(OUT) : $(OBJS)
	$(LD) $(OBJS) -o $(OUT)

bin/%.o : src/%.c
	$(CC) $(CFLAGS) -o $@ $<
	@cp bin/$*.d bin/$*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < bin/$*.d >> bin/$*.P; \
	rm -f bin/$*.d

-include $(OBJS:.o=.P)

clean :
	rm -r bin
