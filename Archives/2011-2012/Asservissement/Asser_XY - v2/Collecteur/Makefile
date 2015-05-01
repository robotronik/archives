# voir http://mad-scientist.net/make/autodep.html

CC = gcc
ifeq ($(PLATFORM),windows)
	TARGETFLAG = -mno-cygwin
	OUT = CollecteurWin.exe
else
	TARGETFLAG = 
	OUT = CollecteurLin.exe
endif
CFLAGS = -MD -c -DCOLLECTEUR -g -Wall $(TARGETFLAG)
LD = gcc $(TARGETFLAG)
OBJS = bin/Main.o bin/AsserGlue.o bin/PSoCGlue.o bin/Acquirer.o bin/Vars.o bin/Asser/motors.o bin/Asser/messageAsser.o bin/Asser/messageCommun.o bin/Asser/init.o bin/Asser/asser.o bin/Asser/debug.o bin/Asser/uart.o bin/Asser/myMath.o bin/Asser/exception.o bin/Asser/AlphaDelta.o bin/Asser/XYAngle.o bin/Asser/callage.o bin/Asser/QEI.o bin/PSoC/asser.o bin/PSoC/init.o bin/PSoC/test_asser.o bin/PSoC/DMA.o

all :
	make init; \
	make $(OUT)

init :
	@mkdir -p bin bin/Asser bin/PSoC

$(OUT) : $(OBJS)
	$(LD) $(OBJS) -o $(OUT)

bin/%.o : src/%.c
	$(CC) $(CFLAGS) -o $@ $<
	@cp bin/$*.d bin/$*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < bin/$*.d >> bin/$*.P; \
	rm -f bin/$*.d

bin/Asser/%.o : ../sources/%.c
	$(CC) $(CFLAGS) -o $@ $<
	@cp bin/Asser/$*.d bin/Asser/$*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < bin/Asser/$*.d >> bin/Asser/$*.P; \
	rm -f bin/Asser/$*.d

bin/PSoC/%.o : ../PSoC/CommandeAsser.cydsn/%.c
	$(CC) $(CFLAGS) -o $@ $<
	@cp bin/PSoC/$*.d bin/PSoC/$*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < bin/PSoC/$*.d >> bin/PSoC/$*.P; \
	rm -f bin/PSoC/$*.d

-include $(OBJS:.o=.P)

clean :
	rm -r bin
