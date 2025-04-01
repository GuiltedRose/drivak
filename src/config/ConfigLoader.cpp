#undef slots
#include <Python.h>
#include "config/ConfigLoader.h"
#include "core/DataPaths.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

DrivakConfig ConfigLoader::load() {
    DrivakConfig config;

    QString configDir = DataPaths::config();
    QString pyPath = configDir + "/config.py";

    if (!QFile::exists(pyPath)) {
        qWarning() << "[Drivak] config.py not found at" << pyPath;
        return config;
    }

    if (!Py_IsInitialized()) {
        Py_Initialize();
    }

    // Prepare Python context
    FILE* file = fopen(pyPath.toStdString().c_str(), "r");
    if (!file) {
        qWarning() << "[Drivak] Failed to open config.py";
        return config;
    }

    PyObject* globals = PyDict_New();
    PyDict_SetItemString(globals, "__builtins__", PyEval_GetBuiltins());

    int result = PyRun_SimpleFile(file, pyPath.toStdString().c_str());
    fclose(file);

    if (result != 0) {
        PyErr_Print();
        qWarning() << "[Drivak] Failed to execute config.py";
        Py_DECREF(globals);
        return config;
    }

    // Read values from the Python globals dict
    auto getBool = [&](const char* key, bool fallback) {
        PyObject* obj = PyDict_GetItemString(globals, key);
        return obj ? PyObject_IsTrue(obj) : fallback;
    };

    auto getString = [&](const char* key, const std::string& fallback) {
        PyObject* obj = PyDict_GetItemString(globals, key);
        if (obj && PyUnicode_Check(obj)) {
            return std::string(PyUnicode_AsUTF8(obj));
        }
        return fallback;
    };

    config.copy_default_mods = getBool("copy_default_mods", config.copy_default_mods);
    config.enable_mods       = getBool("enable_mods", config.enable_mods);
    config.show_tips         = getBool("show_tips", config.show_tips);
    config.theme             = getString("theme", config.theme);

    Py_DECREF(globals);
    return config;
}
