#include "PythonIntegration.h"
#include <stdexcept>
#include <iostream>

std::mutex PythonIntegration::pythonMutex;

PythonIntegration::PythonIntegration() : pModule(nullptr), pClass(nullptr), pInstance(nullptr) {
    initializePython();
}

PythonIntegration::~PythonIntegration() {
    finalizePython();
}

void PythonIntegration::initializePython() {
    std::lock_guard<std::mutex> lock(pythonMutex);
    
    if (!Py_IsInitialized()) {
        Py_Initialize();
    }
    
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./scripts')");
    
    pModule = PyImport_ImportModule("adaptive_frequency_hopping");
    if (!pModule) {
        PyErr_Print();
        throw std::runtime_error("Failed to load adaptive_frequency_hopping module");
    }
    
    pClass = PyObject_GetAttrString(pModule, "AdaptiveFrequencyHopping");
    if (!pClass) {
        throw std::runtime_error("Failed to load AdaptiveFrequencyHopping class");
    }
    
    PyObject *pArgs = PyTuple_Pack(3, PyFloat_FromDouble(1000.0), PyFloat_FromDouble(2000.0), PyLong_FromLong(10));
    pInstance = PyObject_CallObject(pClass, pArgs);
    Py_DECREF(pArgs);
    
    if (!pInstance) {
        throw std::runtime_error("Failed to create AdaptiveFrequencyHopping instance");
    }
}

void PythonIntegration::finalizePython() {
    std::lock_guard<std::mutex> lock(pythonMutex);
    
    Py_XDECREF(pInstance);
    Py_XDECREF(pClass);
    Py_XDECREF(pModule);
    
    if (Py_IsInitialized()) {
        Py_Finalize();
    }
}

PyObject* PythonIntegration::callMethod(const char* methodName, PyObject* pArgs) {
    std::lock_guard<std::mutex> lock(pythonMutex);
    
    PyObject *pResult = PyObject_CallMethod(pInstance, methodName, "(O)", pArgs);
    if (!pResult) {
        PyErr_Print();
        throw std::runtime_error(std::string("Failed to call ") + methodName + " method");
    }
    return pResult;
}

double PythonIntegration::getNextFrequency(const std::vector<double>& noiseLevels) {
    PyObject *pNoiseLevels = PyList_New(noiseLevels.size());
    for (size_t i = 0; i < noiseLevels.size(); ++i) {
        PyList_SetItem(pNoiseLevels, i, PyFloat_FromDouble(noiseLevels[i]));
    }
    
    PyObject *pResult = callMethod("get_next_frequency", pNoiseLevels);
    Py_DECREF(pNoiseLevels);
    
    double result = PyFloat_AsDouble(pResult);
    Py_DECREF(pResult);
    
    return result;
}

void PythonIntegration::updateChannelPerformance(int channel, bool success) {
    PyObject *pArgs = PyTuple_Pack(2, PyLong_FromLong(channel), PyBool_FromLong(success));
    PyObject *pResult = PyObject_CallMethod(pInstance, "update_channel_performance", "(iO)", channel, success ? Py_True : Py_False);
    
    if (!pResult) {
        PyErr_Print();
        std::cerr << "Failed to call update_channel_performance method" << std::endl;
        Py_DECREF(pArgs);
        return;
    }
    
    Py_DECREF(pResult);
    Py_DECREF(pArgs);
}


std::vector<double> PythonIntegration::simulateNoise() {
    PyObject *pResult = PyObject_CallMethod(pInstance, "simulate_noise", NULL);
    if (!pResult) {
        PyErr_Print();
        throw std::runtime_error("Failed to call simulate_noise method");
    }
    
    if (!PyList_Check(pResult)) {
        Py_DECREF(pResult);
        throw std::runtime_error("simulate_noise did not return a list");
    }
    
    std::vector<double> noise;
    Py_ssize_t size = PyList_Size(pResult);
    for (Py_ssize_t i = 0; i < size; ++i) {
        PyObject *item = PyList_GetItem(pResult, i);
        noise.push_back(PyFloat_AsDouble(item));
    }
    
    Py_DECREF(pResult);
    return noise;
}


