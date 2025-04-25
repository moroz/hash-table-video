LDFLAGS += -lcriterion

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	CFLAGS += -I$(shell brew --prefix criterion)/include
	LDFLAGS += -L$(shell brew --prefix criterion)/lib
endif

.PHONY: test
test: table.c test.c
	$(CC) $(CFLAGS) -g $^ -o $@ $(LDFLAGS)
	./test
