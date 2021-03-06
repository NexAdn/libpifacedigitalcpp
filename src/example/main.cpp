#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

#include "../PiFace.hpp"

inline void wait()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

inline void jij(std::thread& t)
{
    if (t.joinable())
        t.join();
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

    std::cout << "Reading input state change events\n";
    bool run{true};
    std::condition_variable cv1, cv2, cv3, cv4;
    std::thread t1{[&run, &pf, &cv1]() {
        std::mutex mtx;
        std::unique_lock<std::mutex> lk{mtx};
        std::condition_variable& cv = cv1;

        pf.registerInputStateChangeNotifier(cv);
        while (run) {
            cv.wait(lk, [&pf, &run]() { return pf.getInput(0) || !run; });
            if (pf.getInput(0))
                std::cout << "Input 0 pressed\n";
            cv.wait(lk, [&pf, &run]() { return !pf.getInput(0) || !run; });
            cv.notify_all();
        }
        pf.unregisterInputStateChangeNotifier(cv);
    }};
    std::thread t2{[&run, &pf, &cv2]() {
        std::mutex mtx;
        std::unique_lock<std::mutex> lk{mtx};
        std::condition_variable& cv = cv2;

        pf.registerInputStateChangeNotifier(cv);
        while (run) {
            cv.wait(lk, [&pf, &run]() { return pf.getInput(1) || !run; });
            if (pf.getInput(1))
                std::cout << "Input 1 pressed\n";
            cv.wait(lk, [&pf, &run]() { return !pf.getInput(1) || !run; });
            cv.notify_all();
        }
        pf.unregisterInputStateChangeNotifier(cv);
    }};
    std::thread t3{[&run, &pf, &cv3]() {
        std::mutex mtx;
        std::unique_lock<std::mutex> lk{mtx};
        std::condition_variable& cv = cv3;

        pf.registerInputStateChangeNotifier(cv);
        while (run) {
            cv.wait(lk, [&pf, &run]() { return pf.getInput(2) || !run; });
            if (pf.getInput(2))
                std::cout << "Input 2 pressed\n";
            cv.wait(lk, [&pf, &run]() { return !pf.getInput(2) || !run; });
            cv.notify_all();
        }
        pf.unregisterInputStateChangeNotifier(cv);
    }};
    std::thread t4{[&run, &pf, &cv4]() {
        std::mutex mtx;
        std::unique_lock<std::mutex> lk{mtx};
        std::condition_variable& cv = cv4;

        pf.registerInputStateChangeNotifier(cv);
        while (run) {
            cv.wait(lk, [&pf, &run]() { return pf.getInput(3) || !run; });
            if (pf.getInput(3))
                std::cout << "Input 3 pressed\n";
            cv.wait(lk, [&pf, &run]() { return !pf.getInput(3) || !run; });
            cv.notify_all();
        }
        pf.unregisterInputStateChangeNotifier(cv);
    }};

    std::cout << "Input listener running.\nPress enter to quit.\n";
    std::cin.ignore();
    std::cout << "Quitting\n";
    run = false;
    cv1.notify_all();
    cv2.notify_all();
    cv3.notify_all();
    cv4.notify_all();
    jij(t1);
    jij(t2);
    jij(t3);
    jij(t4);
    std::cout << "Quit\n";
}
