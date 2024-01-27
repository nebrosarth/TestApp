#pragma once

#include <QtWidgets/QWidget>
#include "ui_TestApp.h"
#include <qthreadpool.h>

QT_BEGIN_NAMESPACE
namespace Ui { class TestAppClass; };
QT_END_NAMESPACE

class TestApp : public QWidget
{
    Q_OBJECT

public:
    TestApp(QWidget *parent = nullptr);
    ~TestApp();
    void CreateTask();
protected:
    void initConnections();

private:
    Ui::TestAppClass *ui;
    QThreadPool m_pool;

};
