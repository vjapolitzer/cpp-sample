## C++ Sample

This sample is from the C++ backend of a Qt Quick based touchscreen interface that runs on an embedded linux board. TemperatureWorker is the worker class that runs in a thread to poll an AVR-based temperature control board over I2C, sending updated an target temperature if it is available from user input and updating the display values on the touchscreen. It inherits from the I2CWorker class, which has the core I2C communication functionality.
