CC = gcc
CFLAGS = -Wall -g
LIBS = -lcurl -lcjson

all: chatbot

chatbot: chatbot.c
	$(CC) $(CFLAGS) -o chatbot chatbot.c $(LIBS)

clean:
	rm -f chatbot