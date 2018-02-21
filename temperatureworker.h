#ifndef TEMPERATUREWORKER_H
#define TEMPERATUREWORKER_H

#include "i2cworker.h"
#include "temperaturecontroller.h"

#define MAIN_PCB_ADDRESS 0x75 // I2C slave address of the heater PCB

class TemperatureController;

class TemperatureWorker : public I2CWorker
{
    Q_OBJECT
public:
    /* Constructor
     * ...Parameters:
     * ......TemperatureController *parent -- controller in parent thread
     * ...Returns:
     * ......Nothing
     */
    TemperatureWorker(TemperatureController *parent);

public slots:
    /* Temperature Poll and Update Slot
     * ...Polls temperature and sends new target temperature when updated in GUI
     * ...Returns:
     * ......Nothing
     */
    void pollAndUpdate();

signals:
    /* invalidTemperature Signal
     * ...Connect to slot handleInvalidTemperature in class that uses TemperatureController
     * ...Returns:
     * ......Nothing
     */
    void invalidTemperature();

private:
    /* Temperature Value Validation Function
     * ...Parameters:
     * ......float temperature -- Temperature value in Celsius
     * ...Returns:
     * ......true if temperature is in valid range, false otherwise
     */
    bool validTemperature(float temperature);

    /* Display Value Update Function
     * ...Update display values in parent controller
     * ...Parameters:
     * ......float temperature -- Current temperature in Celsius
     * ......float target -- Target temperature in Celsius
     * ...Returns:
     * ......Nothing
     */
    void updateDisplayValues(float temperature, float target);

    /* Heater Data Read Function
     * ... Read in current heater data and update GUI display value
     * ...Returns:
     * ......Nothing
     */
    void readHeaterData();

    /* Heater Target Send or Update Function
     * ... Send new target temperature to heater PCB
     * ...Returns:
     * ......Nothing
     */
    void updateTarget();

    TemperatureController *parent; // controller in parent thread
};

#endif // TEMPERATUREWORKER_H
