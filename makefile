CC=gcc
YAZCONFIG=~/Downloads/yaz_5.32.0.orig/yaz-5.32.0/yaz-config
      CFLAGS=`$(YAZCONFIG) --cflags`
      LIBS=`$(YAZCONFIG) --libs`
      z39-demo: z39-demo.o
	$(CC) $(CFLAGS) -o z39-demo z39-demo.o $(LIBS)
      

