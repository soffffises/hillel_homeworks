#include "TemperatureController.h"

TemperatureController::TemperatureController(TemperatureModel* model, QObject *parent)
    : QObject(parent), m_model(model) {}

bool TemperatureController::validateAndConvert(const QString &text, double minValue, double &outValue, QString &errorMsg) {
    if (text.isEmpty() || text == "-") {

        return false;
    }

    QString t = text;
    t.replace(',', '.');
    bool ok = false;
    double val = t.toDouble(&ok);
    if (!ok) {
        errorMsg = "Please enter a number";
        return false;
    }
    if (val < minValue) {
        errorMsg = QString("Value cannot be less than %1").arg(minValue);
        return false;
    }
    outValue = val;
    return true;
}

void TemperatureController::updateFromCelsius(const QString &text) {
    double val = 0.0;
    QString error;
    if (validateAndConvert(text, -273.15, val, error)) {
        m_model->setCelsius(val);
    } else if (!text.isEmpty() && text != "-") {
        emit invalidInput(error);
    }
}

void TemperatureController::updateFromFahrenheit(const QString &text) {
    double val = 0.0;
    QString error;
    if (validateAndConvert(text, -459.67, val, error)) {
        m_model->setFahrenheit(val);
    } else if (!text.isEmpty() && text != "-") {
        emit invalidInput(error);
    }
}

void TemperatureController::updateFromKelvin(const QString &text) {
    double val = 0.0;
    QString error;
    if (validateAndConvert(text, 0.0, val, error)) {
        m_model->setKelvin(val);
    } else if (!text.isEmpty() && text != "-") {
        emit invalidInput(error);
    }
}
