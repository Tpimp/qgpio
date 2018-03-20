TEMPLATE = subdirs

gpio.subdir = gpio
gpio.target = gpio-lib
SUBDIRS += gpio

softpwm.subdir = softpwm
softpwm.target = softpwm-lib
SUBDIRS += softpwm

plugins.subdir = imports
plugins.depends = gpio-lib
plugins.target = plugins-lib
SUBDIRS += plugins
