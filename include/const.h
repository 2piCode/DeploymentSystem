#ifndef CONST_H
#define CONST_H

#include <QObject>
#include <QString>

// Macro to define a constant property
#define DEFINE_CONSTANT(type, name, value)   \
    Q_PROPERTY(type name READ name CONSTANT) \
   public:                                   \
    type name() const { return m_##name; }   \
                                             \
   private:                                  \
    const type m_##name = value;

class Constants : public QObject {
    Q_OBJECT

   public:
    explicit Constants(QObject *parent = nullptr) : QObject(parent) {}

    DEFINE_CONSTANT(int, fontSize, 12)
    DEFINE_CONSTANT(int, stationItemHeigth, 40)
};

#endif
