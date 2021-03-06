#include "src/backend/temperatureworker.h"

TemperatureWorker::TemperatureWorker(TemperatureController *parent) : I2CWorker() {
    this->parent = parent;
}

void TemperatureWorker::pollAndUpdate() {
    while (parent->poll()) {
        readHeaterData();
        if (parent->getNewTargetAvailable()) { // true when user sets new target
            updateTarget();
        }
        sleep(2); // poll once every two seconds
    }
}

void TemperatureWorker::updateDisplayValues(float temperature, float target) {
    parent->setHeaterCurrent(temperature); // update GUI current temp display value
    parent->setHeaterTarget(target); // update GUI target temp display value
}

bool TemperatureWorker::validTemperature(float temperature) {
    return temperature >= 0.0 && temperature <= 280.0;
}

void TemperatureWorker::readHeaterData() {
    float heaterData[2]; // heaterData[0] := heaterTemp, heaterData[1] := heaterTarget
    size_t requestLength = 2;
    char request[requestLength];
    request[0] = 84; // command character for temperature requests
    request[1] = parent->heater(); // set the user selected heater

    // inform heater PCB that incoming read is a temperature read
    I2CWorker::writeData(MAIN_PCB_ADDRESS, request, requestLength);
    // read the temperature data into the heaterData array
    I2CWorker::readData(MAIN_PCB_ADDRESS, (char*)heaterData, 2 * sizeof(float));

    if (validTemperature(heaterData[0]) && validTemperature(heaterData[1])) {
        updateDisplayValues();
    } else {
        emit TemperatureWorker::invalidTemperature();
    }
}

void TemperatureWorker::updateTarget() {
    char *b;
    float newTarget;
    size_t requestLength = 2 + sizeof(float);
    char request[requestLength];

    request[0] = 84; // command character for temperature requests
    request[1] = parent->heater(); // set the user selected heater

    newTarget = parent->heaterTarget();
    b = (char*) &newTarget;
    for (uint8_t i = 0; i < sizeof(float); i++) {
      request[2 + i] = b[i]; // target decomposed into bytes
    }

    // send the new target to the heater PCB
    I2CWorker::writeData(MAIN_PCB_ADDRESS, request, requestLength);

    parent->resetNewTargetAvailable();
}
