# esp8266_bitcoin_ticker

A small OLED bitcoin tricker on esp8266 using bitfinex, tcMenu and IotWebConf.

This script uses few excellent arduino libraries.

1. [TaskManagerIO](https://www.thecoderscorner.com/products/arduino-libraries/taskmanager-io/) - For its taskManager, which is a super way to run "sort of" multiple threads on esp.
2. [IotWebConf](https://github.com/prampec/IotWebConf/) - For their wifi manager which supports storing multiple SSIDs to connect to.
3. Arduino_JSON - for parsing Bitfinex REST API

Ticker reads the public [Bitfinex API](https://docs.bitfinex.com/docs/rest-public) every 10 seconds.


