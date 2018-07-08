#include "PiFace.hpp"

#include <iostream>

#include "pifacedigital.h"

namespace pf
{
PiFace::PiFace(uint8_t hardwareAddress) : hardwareAddress(hardwareAddress)
{
    this->spiFileDescriptor = pifacedigital_open(hardwareAddress);
    pifacedigital_enable_interrupts();
    this->inputListener = std::thread([this]() { this->inputListenerWorker(); });
}

PiFace::~PiFace()
{
    this->inputListenerWorkerRun = false;
    if (this->inputListener.joinable())
        this->inputListener.join();
    pifacedigital_disable_interrupts();
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
    return pifacedigital_read_bit(inputNo, INPUT, this->hardwareAddress) > 0 ? false : true;
}

uint8_t PiFace::bulkGetInput() const
{
    uint8_t res{0x00};
    for (uint8_t i = 0; i < 8; i++)
        res |= this->getInput(i) << i;

    return res;
}

void PiFace::registerInputStateChangeNotifier(std::condition_variable& cv)
{
    this->inputStateChangeNotifiers.push_back(&cv);
}

void PiFace::unregisterInputStateChangeNotifier(std::condition_variable& cv)
{
    for (size_t i = 0; i < this->inputStateChangeNotifiers.size(); i++) {
        if (this->inputStateChangeNotifiers.at(i) == &cv) {
            this->inputStateChangeNotifiers.at(i) = nullptr;
        }
    }
}

void PiFace::inputListenerWorker()
{
    uint8_t currentData{0x0};
    while (this->inputListenerWorkerRun) {
        pifacedigital_wait_for_input(&currentData, 1000, this->hardwareAddress);
        if (currentData != this->lastInput) {
            this->lastInput = currentData;
            for (auto cv : this->inputStateChangeNotifiers) {
                if (cv != nullptr) {
                    cv->notify_all();
                }
            }
        }
    }
}
} // namespace pf
