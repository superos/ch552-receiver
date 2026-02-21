### Makefile

1. open project with arduino ide, compile it, you will get `core.lib`

2. edit `Makefile` with correct path of `APPDATA_LOCAL` and `ARDUINO_BUILD_TEMP`

3. you can use `Makefile` inside `VSCode` from now on.

### CdcHidCombo

1. from `CH55xDuino` library `examples\05.USB\CdcHidCombo\src\CdcHidCombo`

2. input reports handle with arduino `hidInputPolling` @ch552-receiver.ino loop()

3. output reports handle with HID.c `hidOutputCallback` @USBhandler.c USB_EP0_OUT()

4. all changes? `git log --oneline -- src/CdcHidCombo`
