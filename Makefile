CXX=mpixlcxx

CFLAGS=-O3 -g
LIBS=-L/bgsys/drivers/ppcfloor/bgpm/lib -lbgpm -lrt -lstdc++

SRCS = multisend.cxx 

OBJS = 	$(SRCS:.cxx=.o)

TARGET = example

all:    $(TARGET)
		@echo Compilation done.

%.o:%.cxx
		$(CXX) $(CFLAGS) $(LIBS) -c $< -o $@

$(TARGET): $(OBJS) 
		$(CXX) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

clean:
		$(RM) *.o *~ $(MAIN) $(TARGET)

