SHELL = /bin/sh

SUBDIRS	:= lib examples 
TARGETS	:= all install clean 

$(SUBDIRS):
	$(MAKE) -C $@ $(filter $(TARGETS),$(MAKECMDGOALS))

.PHONY: $(TARGETS) $(SUBDIRS)