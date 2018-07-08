#include <iostream>
#include <thread>

#include "PiFace.hpp"
#include "pifacedigital.h"

inline void wait()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main(int argc, char** argv)
{
    std::cout << "Creating a new PiFace object for hardware address 0x00\n";

    pf::PiFace pf{0x00};

    std::cout << "Enabling all outputs\n";
    pf.bulkSetOutput(0xff);
    wait();

    std::cout << "Disabling first output\n";
    pf.setOutput(0, false);
    wait();

    std::cout << "Reading inputs\n";
    wait();
    for (uint8_t i = 0; i < 8; i++) {
        std::cerr << "Input " << static_cast<int>(i) << ": " << pf.getInput(i) << std::endl;
        wait();
    }

    std::cout << "Resetting\n";
    pf.reset();
}
