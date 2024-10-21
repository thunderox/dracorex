#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#
# Modified by REIS0

PLUGIN=dracorex 	

include dpf/Makefile.base.mk

all: dgl plugins gen

# --------------------------------------------------------------

dgl:
ifeq ($(HAVE_OPENGL),true)
	$(MAKE) -C dpf/dgl opengl
endif

plugins: dgl
	$(MAKE) all -C plugins/$(PLUGIN)

ifneq ($(CROSS_COMPILING),true)
gen: plugins dpf/utils/lv2_ttl_generator
	@$(CURDIR)/dpf/utils/generate-ttl.sh
ifeq ($(MACOS),true)
	@$(CURDIR)/dpf/utils/generate-vst-bundles.sh
endif

dpf/utils/lv2_ttl_generator:
	$(MAKE) -C dpf/utils/lv2-ttl-generator
else
gen:
endif

# --------------------------------------------------------------

clean:
	$(MAKE) clean -C dpf/dgl
	$(MAKE) clean -C dpf/utils/lv2-ttl-generator
	$(MAKE) clean -C plugins/$(PLUGIN)
	rm -rf bin build
	rm -f delirium_ui/*.d
	rm -f delirium_ui/*.o
	find -name "*~" -delete

# --------------------------------------------------------------
install:
	cp $(CURDIR)/bin/dracorex.lv2 /usr/lib/lv2 -r
	cp $(CURDIR)/plugins/dracorex/manifest.ttl /usr/lib/lv2/dracorex.lv2
	cp $(CURDIR)/bin/dracorex-dssi /usr/lib/dssi -r
	cp $(CURDIR)/bin/dracorex-dssi.so /usr/lib/dssi
	cp $(CURDIR)/bin/dracorex-vst.so /usr/lib/vst
	chmod +x $(CURDIR)/bin/dracorex
	cp $(CURDIR)/bin/dracorex /usr/bin
	mkdir /home/$(SUDO_USER)/.lv2/dracorex.lv2 -p
	cp $(CURDIR)/plugins/dracorex/waves /home/$(SUDO_USER)/.lv2/dracorex.lv2 -r
	cp $(CURDIR)/plugins/dracorex/presets /home/$(SUDO_USER)/.lv2/dracorex.lv2 -r

# --------------------------------------------------------------
uninstall:
	rm /usr/lib/lv2/dracorex.lv2 -rf
	rm /usr/lib/dssi/dracorex.dssi -rf
	rm /usr/lib/dssi/dracorex-dssi.so -f
	rm /usr/lib/vst/dracorex-vst.so -f
	rm /usr/bin/dracorex
	rm /home/$(SUDO_USER)/.lv2/dracorex.lv2 -r

# --------------------------------------------------------------

.PHONY: plugins
