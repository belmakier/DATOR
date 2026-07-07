all: libNSCL

libNSCL : Type10.cc Type10.hh
	$(CC) $(CFLAGS) -I../ -o $(LIBDIR)/libNSCL.so -shared Type10.cc $(LIBS) -L$(LIBDIR) -lReader

