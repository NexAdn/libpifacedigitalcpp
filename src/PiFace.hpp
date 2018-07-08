#pragma once

#include <stdint.h>

#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

namespace pf
{
class PiFace
{
public:
    PiFace(uint8_t hardwareAddress = 0x00);
    ~PiFace();

    void reset();

    void setOutput(uint8_t outputNo, bool value);
    void bulkSetOutput(uint8_t outputBits);

    bool getInput(uint8_t inputNo) const;
    uint8_t bulkGetInput() const;

    void registerInputStateChangeNotifier(std::condition_variable cv);

private:
    int spiFileDescriptor{-1};
    uint8_t hardwareAddress{0x00};

    std::mutex pfdOutputMutex;

    std::thread inputListener;
    std::vector<std::condition_variable> inputStateChangeNotifiers;
};
} // namespace pf
