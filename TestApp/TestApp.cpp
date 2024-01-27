#include "TestApp.h"

TestApp::TestApp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestAppClass())
{
    ui->setupUi(this);

    ui->splitter->setSizes(QList<int>({ INT_MAX, INT_MAX }));
    ui->splitter_2->setSizes(QList<int>({ INT_MAX, INT_MAX }));

    connect(ui->splitter, &QSplitter::splitterMoved, ui->splitter_2, [&] {
        ui->splitter_2->setSizes(ui->splitter->sizes());
        });
    connect(ui->splitter_2, &QSplitter::splitterMoved, ui->splitter, [&] {
        ui->splitter->setSizes(ui->splitter_2->sizes());
        });
}

TestApp::~TestApp()
{
    delete ui;
}
