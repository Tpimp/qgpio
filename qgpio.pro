enable-tests {
    QTGPIO_BUILD_PARTS = $$QT_BUILD_PARTS
    QTGPIO_BUILD_PARTS *= tests
}

enable-examples {
    QTGPIO_BUILD_PARTS = $$QT_BUILD_PARTS
    QTGPIO_BUILD_PARTS *= examples
}

!unix {
    message(Attempting build on unsupported operating system...)
    message(Currently only Linux distributions are supported)
}

load(qt_parts)

OTHER_FILES += sync.profile
