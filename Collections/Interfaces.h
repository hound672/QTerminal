#ifndef INTERFACES_H
#define INTERFACES_H

#include <QList>
#include <Core/Libs/QComPortThread.h>
#include "Collections/Collections.h"

// ======================================================================
//  ***** Интерфейс описывающий работу главного экрана приложения *****
//  Реализован для возможной дальнешей сменой главного экрана, на QML или т.п
//  Описывает основные методы для работы с окном, реализация которых зависит
//  от конкретного типа.
// ======================================================================
class IGui
{
  
public:
  
  // состояния главного окна
  enum EWindowState {
    stsIdle = 0,
    stsConnecting,
    stsConnected,
  };
  
// ======================================================================
  
public:
  // вирт. деструктор
  virtual ~IGui() {}
  // первичная инициализация класса для работы с главным окном
  virtual void init() = 0;
  // отображение главного окна
  virtual void show() = 0;
  // смена текущего состояния окна
  virtual void setStateWindow(EWindowState newState) = 0;
  // добавлен плученые из порта данные 
  virtual void addData(const QByteArray &data) = 0;
  // ======================================================================
  // вывод инфорации в лог главного окна
  virtual void writeLog(const QString &text) = 0;
  virtual void writeLog(const QString &text, EResultCodes res) = 0;
};

#endif // INTERFACES_H
