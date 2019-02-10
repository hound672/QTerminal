#ifndef INTERFACES_H
#define INTERFACES_H

// Интерфейс описывающий работу главного экрана приложения
class IGui
{
public:
  virtual void init() = 0;
  virtual void show() = 0;
};

#endif // INTERFACES_H
