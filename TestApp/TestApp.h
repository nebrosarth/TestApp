#pragma once

#include <QtWidgets/QWidget>
#include "ui_TestApp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TestAppClass; };
QT_END_NAMESPACE

class TestApp : public QWidget
{
    Q_OBJECT

public:
    TestApp(QWidget *parent = nullptr);
    ~TestApp();

private:
    Ui::TestAppClass *ui;
};
