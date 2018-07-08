#include "PiFace.hpp"

#include <iostream>

#include "pifacedigital.h"

namespace pf
{
PiFace::PiFace(uint8_t hardwareAddress) : hardwareAddress(hardwareAddress)
{
    this->spiFileDescriptor = pifacedigital_open(hardwareAddress);
}

PiFace::~PiFace()
{
    pifacedigital_close(this->hardwareAddress);
}

void PiFace::reset()
{
    this->bulkSetOutput(0x00);
}

void PiFace::setOutput(uint8_t outputNo, bool value)
{
    std::lock_guard<std::mutex> lock(this->pfdOutputMutex);
    pifacedigital_write_bit(static_cast<uint8_t>(value), outputNo, OUTPUT, this->hardwareAddress);
}

void PiFace::bulkSetOutput(uint8_t outputBits)
{
    for (uint8_t i = 0; i < 8; i++)
        this->setOutput(i, outputBits & (0x1 << i));
}

bool PiFace::getInput(uint8_t inputNo) const
{
    return pifacedigital_read_bit(inputNo, INPUT, this->hardwareAddress) > 0 ? true : false;
}

uint8_t PiFace::bulkGetInput() const
{
    uint8_t res{0x00};
    for (uint8_t i = 0; i < 8; i++)
        res |= this->getInput(i) << i;

    return res;
}
} // namespace pf
