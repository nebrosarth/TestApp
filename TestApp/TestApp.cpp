#include "TestApp.h"

TestApp::TestApp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestAppClass())
{
    ui->setupUi(this);
}

TestApp::~TestApp()
{
    delete ui;
}
