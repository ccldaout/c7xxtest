-include ../config.c7xxtest.mk
-include config.c7xxtest.mk

SHELL = /bin/bash

MAKE_JOBS ?= 8

C7ROOT ?= $(HOME)/.ccldaout
LIBDIR ?= $(C7ROOT)/lib
INCDIR ?= $(C7ROOT)/include
PY3DIR ?= $(C7ROOT)/python3

TMPDIR ?= $(HOME)/tmp/c7xxtest/$(CXX)
OBJDIR  = $(TMPDIR)/obj
DBGDIR  = $(TMPDIR)/gdb
BINDIR  = $(TMPDIR)/bin

CXX      ?= c++
CXX_STD  ?= c++17
CXXFLAGS_ = -std=$(CXX_STD) -I$(INCDIR) -Wall -Werror
 LDFLAGS_ = -L$(LIBDIR) -Wl,-rpath,$(LIBDIR)
  LDLIBS_ = -lc7++ -lpthread

ifeq "$(wildcard $(C7ROOT))" ""
  $(error "Please make C7ROOT=<root of libc7++>")
endif

ifeq "$(wildcard .DEBUG)" ""
  CXXFLAGS_ += -O3
  ECHO_BEG = "\\x1b[32m"
  ECHO_END = "\\x1b[0m"
else
  CXXFLAGS_ += -O0 -g2 -DC7_DEBUG_
  OBJDIR = $(DBGDIR)
  LIBDIR = $(DBGDIR)
  BINSFX = .g
  ECHO_BEG = "\\x1b[33m"
  ECHO_END = "\\x1b[0m"
endif

CXXFLAGS_ += $(CXXFLAGS)

#

ifeq (,$(findstring s,$(MAKEFLAGS)))
  ECHO = /bin/echo -e
else
  ECHO = :
endif

define cmd
  @$(ECHO) $(ECHO_BEG)$1$(ECHO_END)
  @$1
endef

define tests
  $(addsuffix $(BINSFX),$(addprefix $(BINDIR)/,$(patsubst $1/%.cpp,$2%,$(wildcard $1/*.cpp))))
endef

# 

$(BINDIR)/%: %.cpp
	$(call cmd,$(CXX) -g $(CXXFLAGS_) $(LDFLAGS_) -o $@ $< $(LDLIBS_))

$(BINDIR)/%$(BINSFX): %.cpp
	$(call cmd,$(CXX) -g $(CXXFLAGS_) $(LDFLAGS_) -o $@ $< $(LDLIBS_))

# nseq

$(BINDIR)/c7nseq_%: nseq/%.cpp
	$(call cmd,$(CXX) -g $(CXXFLAGS_) $(LDFLAGS_) -o $@ $< $(LDLIBS_))

$(BINDIR)/c7nseq_%$(BINSFX): nseq/%.cpp
	$(call cmd,$(CXX) -g $(CXXFLAGS_) $(LDFLAGS_) -o $@ $< $(LDLIBS_))

#

TESTPROGS := $(call tests,.,) $(call tests,nseq,c7nseq_)
PREPRO    := $(shell python3 $(PY3DIR)/setup_dispatcher.py c7event_*.cpp 2>/dev/null)

.PHONY: build
build:
	@make -j$(MAKE_JOBS) --no-print-directory tests
	@[[ $(BINDIR) -ef $(HOME)/tmp/bin ]] || ln -sf $(BINDIR)/* $(HOME)/tmp/bin/

.PHONY: tests
tests: $(TESTPROGS)

.PHONY: debug
debug:
	@mkdir -p .DEBUG
	@make --no-print-directory

.PHONY: nodebug
nodebug:
	@rm -fr .DEBUG
	@make --no-print-directory

.PHONY: depend
depend:
	@rm -f $(OBJDIR)/Makefile.dep.*
	@for cpp in *.cpp; do \
	  prog=$${cpp##*/}; \
	  c++ -c $(CXXFLAGS_) -MM -MT '$$(BINDIR)'/$${prog%.cpp} $${cpp} >| $(OBJDIR)/Makefile.dep.$${cpp##*/}; \
	done
	@for cpp in nseq/*.cpp; do \
	  prog=$${cpp##*/}; \
	  c++ -c $(CXXFLAGS_) -MM -MT '$$(BINDIR)'/c7nseq_$${prog%.cpp} $${cpp} >| $(OBJDIR)/Makefile.dep.$${cpp##*/}; \
	done
	@cat $(OBJDIR)/Makefile.dep.* | sed 's^$(INCDIR)^$$(INCDIR)^g' >| Makefile.dep

.PHONY: init
init:
	@mkdir -p $(TMPDIR) $(BINDIR) $(OBJDIR) $(sort $(dir $(LIBOBJ)))

.PHONY: clean
clean:
	rm -f $(TESTPROGS)
	rm -f $(OBJDIR)/*.o
	rm -f $(OBJDIR)/?*/*.o

.PHONY: push
push:
	git push -f
	git push -f github

.PHONY: pull
pull:
	git pull; git fetch github; git fetch -f --tags

.PHONY: help
help:
	@$(ECHO) $(ECHO_BEG)"Usage: make [CXX=clang++] [ debug | nodebug | depend | init | clean | push ]"$(ECHO_END)


-include Makefile.dep
