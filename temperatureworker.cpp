#include "src/backend/temperatureworker.h"

TemperatureWorker::TemperatureWorker(TemperatureController *parent) : I2CWorker() {
    this->parent = parent;
}

void TemperatureWorker::pollTemp() {
    while (parent->poll()) {
        readHeaterData();
        setTarget();
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
    size_t requestLength = 2
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

void TemperatureWorker::setTarget() {
    char *b;
    float newTarget;
    size_t requestLength = 2 + sizeof(float);
    char request[requestLength];
    request[0] = 84; // command character for temperature requests

    if (parent->getNewHeaterTarget()) { // true when user sets new target
        newTarget = parent->heaterTarget();
        request[1] = parent->heater(); // set the user selected heater

        b = (char*) &newTarget;
        for (uint8_t i = 0; i < sizeof(float); i++) { // put target into byte array
          request[2 + i] = b[i];
        }

        // send the new target to the heater PCB
        I2CWorker::writeData(MAIN_PCB_ADDRESS, request, requestLength);

        parent->resetNewHeaterTarget()
    }
}
