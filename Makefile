
.PHONY: all clean

all:sender recv

CPP = g++
RM = rm -f

SEND_O = CUDP.o sender.o 
RECV_O = CUDP.o recv.o
CPP_ARGS = -c

%.o:%.cpp
	$(CPP) $(CPP_ARGS) $^ -o $@

sender:$(SEND_O) 
	$(CPP) $^ -o $@

recv:$(RECV_O)
	$(CPP) $^ -o $@

clean:
	$(RM) sender recv


