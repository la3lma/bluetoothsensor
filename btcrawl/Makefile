
CMDDIR:=cmd
INTERNALDIR:=internal
TESTDIR:=test
SOURCES:=$(shell find  $(CMDDIR)  -name '*.go') $(shell find  $(CMDDIR)  -name '*.sql')
TESTSRC:=$(shell find  $(TESTDIR) -name '*.go') $(shell find  $(TESTDIR) -name '*.sql')

zot:
	echo $(SOURCES)

build: btcrawl

btcrawl: $(SOURCES) 
	go build cmd/btcrawl/btcrawl.go

tests:  $(SOURCES) $(TESTSRC) 
	go test ./...
	touch tests 


