### Makefile

1. open project with arduino ide, compile it, you will get `core.lib`

2. edit `Makefile` with correct path of `APPDATA_LOCAL` and `ARDUINO_BUILD_TEMP`

3. you can use `Makefile` inside `VSCode` from now on.

### CdcHidCombo

1. from [`CH55xDuino`](https://github.com/DeqingSun/ch55xduino) library [`examples\05.USB\CdcHidCombo\src\CdcHidCombo`](https://github.com/DeqingSun/ch55xduino/tree/ch55xduino/ch55xduino/ch55x/libraries/Generic_Examples/examples/05.USB/CdcHidCombo/src/CdcHidCombo)

2. input reports handle with HID.c `hidInputPolling` @[*loop()*](/ch552-receiver.ino#L10)

3. output reports handle with HID.c `hidOutputCallback` @[*USB_EP0_OUT()*](/src/CdcHidCombo/USBhandler.c#L349)

4. all changes? `git log --oneline -- src/CdcHidCombo`

### Other Receivers
- [wagiminator/CH552-USB-NRF](https://github.com/wagiminator/CH552-USB-NRF)
