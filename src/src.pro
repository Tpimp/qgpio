TEMPLATE = subdirs

gpio.subdir = gpio
gpio.target = gpio-lib
SUBDIRS += gpio

softpwm {
  softpwm.subdir = softpwm
  softpwm.target = softpwm-lib
  SUBDIRS += softpwm
}

qml-plugin {
  plugins.subdir = imports
  plugins.depends = gpio-lib
  plugins.target = plugins-lib
  SUBDIRS += plugins
}
