#ifndef TEMPERATUREMODEL_H
#define TEMPERATUREMODEL_H

#include <QObject>

class TemperatureModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(double celsius READ celsius WRITE setCelsius NOTIFY temperatureChanged)
    Q_PROPERTY(double fahrenheit READ fahrenheit WRITE setFahrenheit NOTIFY temperatureChanged)
    Q_PROPERTY(double kelvin READ kelvin WRITE setKelvin NOTIFY temperatureChanged)

public:
    explicit TemperatureModel(QObject *parent = nullptr);

    double celsius() const;
    void setCelsius(double value);

    double fahrenheit() const;
    void setFahrenheit(double value);

    double kelvin() const;
    void setKelvin(double value);

signals:
    void temperatureChanged();

private:
    double m_celsius;
    double m_fahrenheit;
    double m_kelvin;

    void updateFromCelsius();
    void updateFromFahrenheit();
    void updateFromKelvin();
};

#endif
