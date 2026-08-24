CC = gcc
CFLAGS = -Wall -O2

TARGETS = dllsniffer dllsniffer2 dllsniffer_pro

all: $(TARGETS)

dllsniffer: dllsniffer.c
	$(CC) $(CFLAGS) $< -o $@

dllsniffer2: dllsniffer2.c
	$(CC) $(CFLAGS) $< -o $@

dllsniffer_pro: dllsniffer_pro.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGETS)

.PHONY: all clean
