This is documentation for updating ESP32-S3 firmware from HTTP / Public Server by using GitHub Pages.

What you need to prepare in public storage (VPS/Another Public Server):
- firwmare_x.bin --> 'x' indicates version number, i.e. firmware_1.0, firmware_1.1, firmware_2.7, so on.
- firmware.json --> let ESP32 know there is an update

What if the ESP32 turning off while updating:
- fail to update the latest firmware
- turn back to older version

When you program the firmware:
- ensure check_update_firmware() written
- provide an option for rolling back to older version
