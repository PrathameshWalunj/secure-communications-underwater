
#ifndef PYTHON_INTEGRATION_H
#define PYTHON_INTEGRATION_H

#include <Python.h>
#include <vector>
#include <string>
#include <mutex>

class PythonIntegration {
public:
    PythonIntegration();
    ~PythonIntegration();
    
    double getNextFrequency(const std::vector<double>& noiseLevels);
    void updateChannelPerformance(int channel, bool success);
    std::vector<double> simulateNoise();

private:
    static std::mutex pythonMutex;
    PyObject *pModule, *pClass, *pInstance;
    
    void initializePython();
    void finalizePython();
    PyObject* callMethod(const char* methodName, PyObject* pArgs);
};




#endif
