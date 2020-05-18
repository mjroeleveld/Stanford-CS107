##
## Makefile for our RSS News Feed Aggregator (MT version)
##

## Set the compiler to be a known ANSI compliant one
CC = gcc

## adding the '-DNDEBUG' defines the CT symbol NDEBUG which
## suppresses all the asserts and what not. You'd do that for
## a production build, but it is a good idea to leave it in
## for the purposes of this class.
DFLAG = ## -DNDEBUG

CFLAGS = -D_REENTRANT -g -Wall -D__ostype_is_$(OSTYPE)__ -std=gnu99 -Wno-unused-function $(DFLAG)
LDFLAGS = lib/librssnews.a lib/thread_107.a -lcurl -lexpat
PFLAGS= -linker=/usr/pubsw/bin/ld -best-effort -threads=yes -max-threads=1000

SRCS = rss-news-search.c
OBJS = $(SRCS:.c=.o)
TARGET = rss-news-search
TARGET-PURE = rss-news-search.purify.bin
TARGET-PURE-SCRIPT = rss-news-search.purify

default : $(TARGET)

rss-news-search : $(OBJS)
	$(CC) $(OBJS) $(CFLAGS)$(LDFLAGS) -o $@

pure : $(TARGET-PURE) $(TARGET-PURE-SCRIPT)

rss-news-search.purify :
	echo '#!'`which bash` > $@
	echo 'export LD_LIBRARY_PATH=$${LD_LIBRARY_PATH}:/tmp/rational.`whoami`/`hostname`/usr/local/lib' >> $@
	echo 'echo Library search path is $$LD_LIBRARY_PATH' >> $@
	echo "./rss-news-search.purify.bin" >> $@
	chmod u+x $@

rss-news-search.purify.bin : $(OBJS)
	purify -cache-dir=/tmp $(PFLAGS) $(CC) $(OBJS) $(CFLAGS)$(LDFLAGS) -o $@

# The dependencies below make use of make's default rules,
# under which a .o automatically depends on its .c and
# the action taken uses the $(CC) and $(CFLAGS) variables.
# These lines describe a few extra dependencies involved

clean : 
	@echo "Removing all object files..."
	/bin/rm -f *.o a.out core $(TARGET) $(TARGET-PURE) $(TARGET-PURE-SCRIPT)

TAGS : $(SRCS) $(HDRS)
	etags -t $(SRCS) $(HDRS)
