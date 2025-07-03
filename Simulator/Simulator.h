// Приведенный ниже блок ifdef — это стандартный метод создания макросов, упрощающий процедуру
// экспорта из библиотек DLL. Все файлы данной DLL скомпилированы с использованием символа SIMULATOR_EXPORTS
// Символ, определенный в командной строке. Этот символ не должен быть определен в каком-либо проекте,
// использующем данную DLL. Благодаря этому любой другой проект, исходные файлы которого включают данный файл, видит
// функции SIMULATOR_API как импортированные из DLL, тогда как данная DLL видит символы,
// определяемые данным макросом, как экспортированные.
#pragma once
#ifdef SIMULATOR_EXPORTS
#define SIMULATOR_API __declspec(dllexport)
#else
#define SIMULATOR_API __declspec(dllimport)
#endif

#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <functional>

class SIMULATOR_API VoltageSimulator {
public:
    VoltageSimulator();
    ~VoltageSimulator();

    void start();
    void stop();
    bool isRunning() const;

    void setInterval(unsigned int milliseconds);
    unsigned int getInterval() const;

    void setMinVoltage(double value);
    void setMaxVoltage(double value);
    void setStepSize(double value);

    double getCurrentVoltage() const;

    typedef void (*VoltageUpdateCallback)(double voltage);
    void registerCallback(VoltageUpdateCallback callback);

private:
    void run();

    std::atomic<bool> running;
    std::thread workerThread;
    mutable std::mutex mutex;

    unsigned int interval;
    double minVoltage;
    double maxVoltage;
    double stepSize;
    double currentVoltage;
    bool increasing;
    VoltageUpdateCallback callback;
};

class SIMULATOR_API TemperatureSimulator {
public:
    TemperatureSimulator();
    ~TemperatureSimulator();

    void start();
    void stop();
    bool isRunning() const;

    void setInterval(unsigned int milliseconds);
    unsigned int getInterval() const;

    void setMinTemperature(double value);
    void setMaxTemperature(double value);
    void setStepSize(double value);

    double getCurrentTemperature() const;

    typedef void (*TemperatureUpdateCallback)(double temperature);
    void registerCallback(TemperatureUpdateCallback callback);

private:
    void run();

    std::atomic<bool> running;
    std::thread workerThread;
    mutable std::mutex mutex;

    unsigned int interval;
    double minTemperature;
    double maxTemperature;
    double stepSize;
    double currentTemperature;
    bool increasing;
    TemperatureUpdateCallback callback;
};

// C-интерфейс для экспорта
extern "C" {
    // Voltage functions
    SIMULATOR_API VoltageSimulator* createVoltageSimulator();
    SIMULATOR_API void destroyVoltageSimulator(VoltageSimulator* instance);
    SIMULATOR_API void startVoltageSimulator(VoltageSimulator* instance);
    SIMULATOR_API void stopVoltageSimulator(VoltageSimulator* instance);
    SIMULATOR_API void setVoltageInterval(VoltageSimulator* instance, unsigned int milliseconds);
    SIMULATOR_API void setVoltageRange(VoltageSimulator* instance, double min, double max);
    SIMULATOR_API void setVoltageStep(VoltageSimulator* instance, double step);
    SIMULATOR_API double getCurrentVoltage(VoltageSimulator* instance);
    SIMULATOR_API void registerVoltageCallback(VoltageSimulator* instance, VoltageSimulator::VoltageUpdateCallback callback);

    // Temperature functions
    SIMULATOR_API TemperatureSimulator* createTemperatureSimulator();
    SIMULATOR_API void destroyTemperatureSimulator(TemperatureSimulator* instance);
    SIMULATOR_API void startTemperatureSimulator(TemperatureSimulator* instance);
    SIMULATOR_API void stopTemperatureSimulator(TemperatureSimulator* instance);
    SIMULATOR_API void setTemperatureInterval(TemperatureSimulator* instance, unsigned int milliseconds);
    SIMULATOR_API void setTemperatureRange(TemperatureSimulator* instance, double min, double max);
    SIMULATOR_API void setTemperatureStep(TemperatureSimulator* instance, double step);
    SIMULATOR_API double getCurrentTemperature(TemperatureSimulator* instance);
    SIMULATOR_API void registerTemperatureCallback(TemperatureSimulator* instance, TemperatureSimulator::TemperatureUpdateCallback callback);
}
