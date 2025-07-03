// Simulator.cpp : Определяет экспортируемые функции для DLL.
//

#include "Simulator.h"


VoltageSimulator::VoltageSimulator()
    : running(false), interval(1000), minVoltage(0), maxVoltage(10),
    stepSize(0.1), currentVoltage(0), increasing(true) {
}

VoltageSimulator::~VoltageSimulator() {
    stop();
}

void VoltageSimulator::start() {
    if (!running) {
        running = true;
        workerThread = std::thread(&VoltageSimulator::run, this);
    }
}

void VoltageSimulator::stop() {
    if (running) {
        running = false;
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }
}

bool VoltageSimulator::isRunning() const {
    return running;
}

void VoltageSimulator::setInterval(unsigned int milliseconds) {
    std::lock_guard<std::mutex> lock(mutex);
    interval = milliseconds;
}

unsigned int VoltageSimulator::getInterval() const {
    return interval;
}

void VoltageSimulator::setMinVoltage(double value) {
    std::lock_guard<std::mutex> lock(mutex);
    minVoltage = value;
}

void VoltageSimulator::setMaxVoltage(double value) {
    std::lock_guard<std::mutex> lock(mutex);
    maxVoltage = value;
}

void VoltageSimulator::setStepSize(double value) {
    std::lock_guard<std::mutex> lock(mutex);
    stepSize = value;
}

double VoltageSimulator::getCurrentVoltage() const {
    std::lock_guard<std::mutex> lock(mutex);
    return currentVoltage;
}

void VoltageSimulator::registerCallback(VoltageUpdateCallback callback) {
    this->callback = callback;
}

void VoltageSimulator::run() {
    while (running) {
        auto start = std::chrono::steady_clock::now();

        {
            std::lock_guard<std::mutex> lock(mutex);

            if (increasing) {
                currentVoltage += stepSize;
                if (currentVoltage >= maxVoltage) {
                    currentVoltage = maxVoltage;
                    increasing = false;
                }
            }
            else {
                currentVoltage -= stepSize;
                if (currentVoltage <= minVoltage) {
                    currentVoltage = minVoltage;
                    increasing = true;
                }
            }

            if (callback) {
                callback(currentVoltage);
            }
        }

        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        auto sleepTime = interval - elapsed;

        if (sleepTime > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        }
    }
}

TemperatureSimulator::TemperatureSimulator()
    : running(false), interval(1000), minTemperature(20), maxTemperature(100),
    stepSize(0.5), currentTemperature(20), increasing(true) {
}

TemperatureSimulator::~TemperatureSimulator() {
    stop();
}

void TemperatureSimulator::start() {
    if (!running) {
        running = true;
        workerThread = std::thread(&TemperatureSimulator::run, this);
    }
}

void TemperatureSimulator::stop() {
    if (running) {
        running = false;
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }
}

bool TemperatureSimulator::isRunning() const {
    return running;
}

void TemperatureSimulator::setInterval(unsigned int milliseconds) {
    std::lock_guard<std::mutex> lock(mutex);
    interval = milliseconds;
}

unsigned int TemperatureSimulator::getInterval() const {
    return interval;
}

void TemperatureSimulator::setMinTemperature(double value) {
    std::lock_guard<std::mutex> lock(mutex);
    minTemperature = value;
}

void TemperatureSimulator::setMaxTemperature(double value) {
    std::lock_guard<std::mutex> lock(mutex);
    maxTemperature = value;
}

void TemperatureSimulator::setStepSize(double value) {
    std::lock_guard<std::mutex> lock(mutex);
    stepSize = value;
}

double TemperatureSimulator::getCurrentTemperature() const {
    std::lock_guard<std::mutex> lock(mutex);
    return currentTemperature;
}

void TemperatureSimulator::registerCallback(TemperatureUpdateCallback callback) {
    this->callback = callback;
}

void TemperatureSimulator::run() {
    while (running) {
        auto start = std::chrono::steady_clock::now();

        {
            std::lock_guard<std::mutex> lock(mutex);

            if (increasing) {
                currentTemperature += stepSize;
                if (currentTemperature >= maxTemperature) {
                    currentTemperature = maxTemperature;
                    increasing = false;
                }
            }
            else {
                currentTemperature -= stepSize;
                if (currentTemperature <= minTemperature) {
                    currentTemperature = minTemperature;
                    increasing = true;
                }
            }

            if (callback) {
                callback(currentTemperature);
            }
        }

        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        auto sleepTime = interval - elapsed;

        if (sleepTime > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        }
    }
}

// C-интерфейс для Voltage
extern "C" {
    VoltageSimulator* createVoltageSimulator() { return new VoltageSimulator(); }
    void destroyVoltageSimulator(VoltageSimulator* instance) { delete instance; }
    void startVoltageSimulator(VoltageSimulator* instance) { instance->start(); }
    void stopVoltageSimulator(VoltageSimulator* instance) { instance->stop(); }
    void setVoltageInterval(VoltageSimulator* instance, unsigned int milliseconds) { instance->setInterval(milliseconds); }
    void setVoltageRange(VoltageSimulator* instance, double min, double max) {
        instance->setMinVoltage(min);
        instance->setMaxVoltage(max);
    }
    void setVoltageStep(VoltageSimulator* instance, double step) { instance->setStepSize(step); }
    double getCurrentVoltage(VoltageSimulator* instance) { return instance->getCurrentVoltage(); }
    void registerVoltageCallback(VoltageSimulator* instance, VoltageSimulator::VoltageUpdateCallback callback) {
        instance->registerCallback(callback);
    }
}

// C-интерфейс для Temperature
extern "C" {
    TemperatureSimulator* createTemperatureSimulator() { return new TemperatureSimulator(); }
    void destroyTemperatureSimulator(TemperatureSimulator* instance) { delete instance; }
    void startTemperatureSimulator(TemperatureSimulator* instance) { instance->start(); }
    void stopTemperatureSimulator(TemperatureSimulator* instance) { instance->stop(); }
    void setTemperatureInterval(TemperatureSimulator* instance, unsigned int milliseconds) { instance->setInterval(milliseconds); }
    void setTemperatureRange(TemperatureSimulator* instance, double min, double max) {
        instance->setMinTemperature(min);
        instance->setMaxTemperature(max);
    }
    void setTemperatureStep(TemperatureSimulator* instance, double step) { instance->setStepSize(step); }
    double getCurrentTemperature(TemperatureSimulator* instance) { return instance->getCurrentTemperature(); }
    void registerTemperatureCallback(TemperatureSimulator* instance, TemperatureSimulator::TemperatureUpdateCallback callback) {
        instance->registerCallback(callback);
    }
}

