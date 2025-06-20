#include "TemperatureModel.h"

TemperatureModel::TemperatureModel(QObject *parent)
    : QObject(parent), m_celsius(0.0), m_fahrenheit(32.0), m_kelvin(273.15) {}

double TemperatureModel::celsius() const { return m_celsius; }
void TemperatureModel::setCelsius(double value) {
    if (qFuzzyCompare(m_celsius, value))
        return;

    m_celsius = value;
    updateFromCelsius();
    emit temperatureChanged();
}

double TemperatureModel::fahrenheit() const { return m_fahrenheit; }
void TemperatureModel::setFahrenheit(double value) {
    if (qFuzzyCompare(m_fahrenheit, value))
        return;

    m_fahrenheit = value;
    updateFromFahrenheit();
    emit temperatureChanged();
}

double TemperatureModel::kelvin() const { return m_kelvin; }
void TemperatureModel::setKelvin(double value) {
    if (qFuzzyCompare(m_kelvin, value))
        return;

    m_kelvin = value;
    updateFromKelvin();
    emit temperatureChanged();
}

void TemperatureModel::updateFromCelsius() {
    m_fahrenheit = m_celsius * 9.0 / 5.0 + 32.0;
    m_kelvin = m_celsius + 273.15;
}

void TemperatureModel::updateFromFahrenheit() {
    m_celsius = (m_fahrenheit - 32.0) * 5.0 / 9.0;
    m_kelvin = m_celsius + 273.15;
}

void TemperatureModel::updateFromKelvin() {
    m_celsius = m_kelvin - 273.15;
    m_fahrenheit = m_celsius * 9.0 / 5.0 + 32.0;
}
