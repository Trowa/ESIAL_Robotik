# Copyright 2011 Adam Green (http://mbed.org/users/AdamGreen/)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
PROJECT=Asservissement
INCDIRS=
LIBS_PREFIX=
LIBS_SUFFIX=
GPFLAGS=-DDATE_COMPIL='"$(shell date)"' -DAUTEUR_COMPIL='"$(USER)"' -DGIT_VERSION='"$(shell git describe --dirty --always)"'
PYTHON?=/usr/bin/python
NEWLIB_NANO=0

include $(GCC4MBED_DIR)/build/gcc4mbed.mk

configs: $(wildcard config.*.txt)
config.%.txt: asserv/config/params.h
	@echo Regénération $@
	$(Q) $(PYTHON) gen_config.py $< $@

deploy-default:
	$(Q) $(subst PROJECT.bin,config.default.txt,$(LPC_DEPLOY))

deploy-%:
	$(Q) $(eval DIR=$(shell mktemp -d))
	$(Q) cp config.$(subst deploy-,,$@).txt $(DIR)/config.txt
	$(Q) $(subst PROJECT.bin,$(DIR)/config.txt,$(LPC_DEPLOY))
	$(Q) rm -rf $(DIR)

all: $(DEVICES) configs
term:
	picocom /dev/ttyACM0 -b 230400 --imap lfcrlf

.PHONY: all term configs
