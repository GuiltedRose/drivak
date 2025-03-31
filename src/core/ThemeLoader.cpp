#include "core/ThemeLoader.h"
#include "core/DataPaths.h"
#include "config/example_theme.h"
#include <Python.h>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>

ThemeConfig ThemeLoader::load(const std::string& themeName) {
    ThemeConfig config;
    config.name = themeName;

    QString configDir = DataPaths::config();
    QString themePyPath = QDir(configDir).filePath("theme.py");

    // Auto-generate theme.py if missing
    if (!QFile::exists(themePyPath)) {
        QFile file(themePyPath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << QString::fromStdString(exampleThemePy);
            file.close();
            qDebug() << "[ThemeLoader] Created default theme.py at" << themePyPath;
        }
    }

    if (!QFile::exists(themePyPath)) {
        qWarning() << "[ThemeLoader] Failed to find theme.py after creation:" << themePyPath;
        return config;
    }

    if (!Py_IsInitialized()) {
        Py_Initialize();
    }

    FILE* file = fopen(themePyPath.toStdString().c_str(), "r");
    if (!file) {
        qWarning() << "[ThemeLoader] Failed to open theme.py";
        return config;
    }

    PyObject* globals = PyDict_New();
    PyDict_SetItemString(globals, "__builtins__", PyEval_GetBuiltins());

    int result = PyRun_SimpleFile(file, themePyPath.toStdString().c_str());
    fclose(file);

    if (result != 0) {
        PyErr_Print();
        qWarning() << "[ThemeLoader] Failed to execute theme.py";
        Py_DECREF(globals);
        return config;
    }

    // Pull values from theme.py
    auto getStr = [&](const char* key, const std::string& fallback) {
        PyObject* obj = PyDict_GetItemString(globals, key);
        if (obj && PyUnicode_Check(obj)) {
            return std::string(PyUnicode_AsUTF8(obj));
        }
        return fallback;
    };

    auto getInt = [&](const char* key, int fallback) {
        PyObject* obj = PyDict_GetItemString(globals, key);
        return (obj && PyLong_Check(obj)) ? static_cast<int>(PyLong_AsLong(obj)) : fallback;
    };

    config.name             = getStr("name", config.name);
    config.primary_color    = getStr("primary_color", config.primary_color);
    config.background_color = getStr("background_color", config.background_color);
    config.font             = getStr("font", config.font);
    config.font_size        = getInt("font_size", config.font_size);

    Py_DECREF(globals);
    return config;
}
