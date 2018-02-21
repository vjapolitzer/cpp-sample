#ifndef I2CWORKER_H
#define I2CWORKER_H

#include <QObject>
#include "i2c.h"

class I2CWorker : public QObject
{
    Q_OBJECT
public slots:
    /* writeData Slot
     * ...Connected to I2CController signal signalWriteData
     * ...Emits commResult signal
     * ...Parameters:
     * ......const int address -- the I2C address of the slave
     * ......const char* data -- pointer to array of data to write
     * ......const size_t numBytes --number of bytes to be written
     * ...Returns:
     * ......Nothing
     */
    void writeData(const int address, const char *data, const size_t numBytes);

    /* readData Slot
     * ...Connected to I2CController slsignalot signalReadData
     * ...Emits commResult signal
     * ...Parameters:
     * ......const int address -- the I2C address of the slave
     * ......char* data -- pointer to array of data to read
     * ......const size_t numBytes --number of bytes to be read
     * ...Returns:
     * ......Nothing
     */
    void readData(const int address, char *data, const size_t numBytes);

signals:
    /* writeResult Signal
     * ...Connected to I2CController slot handleWriteResult
     * ...Parameters:
     * ......const bool success -- true if no I2C write error, false if error occurs
     * ...Returns:
     * ......Nothing
     */
    void writeResult(const bool success);

    /* readResult Signal
     * ...Connected to I2CController slot handleReadResult
     * ...Parameters:
     * ......const bool success -- true if no I2C read error, false if error occurs
     * ...Returns:
     * ......Nothing
     */
    void readResult(const bool success);

private:
    i2c *board = nullptr; // i2c object that handles communication

    /* joinBus
     * ...Parameters:
     * ......const int address -- the I2C address of the slave
     * ...Returns:
     * ......Nothing
     * If not already connected, connects to the I2C bus with slave ADDRESS
     * If already connected, sets I2C slave address to ADDRESS
     */
    void joinBus(const int address);
};

#endif // I2CWORKER_H
