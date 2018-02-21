#include "i2cworker.h"

void I2CWorker::writeData(const int address, const char *data, const size_t numBytes)
{
    uint8_t attempts = 0;
    joinBus(address);
    while (!board->i2cWrite(data, numBytes) && attempts < 10) {
        attempts++;
    }
    emit I2CWorker::writeResult(attempts < 10);
}

void I2CWorker::readData(const int address, char *data, const size_t numBytes)
{
    joinBus(address);
    emit I2CWorker::readResult(board->i2cRead(data, numBytes));
}

void I2CWorker::joinBus(const int address)
{
    if (!board) {
        board = new i2c(address);
    } else if (board->getAddress() != address) {
        board->setAddress(address);
    }
}
