#include "TestApp.h"
#include "TaskView.h"
#include "Task.h"

TestApp::TestApp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TestAppClass())
{
    ui->setupUi(this);

    ui->splitter->setSizes(QList<int>({ INT_MAX, INT_MAX }));
    ui->splitter_2->setSizes(QList<int>({ INT_MAX, INT_MAX }));

    initConnections();
}

TestApp::~TestApp()
{
    delete ui;
}

void TestApp::CreateTask()
{
    std::shared_ptr<Task> ptr = std::make_shared<Task>();
    auto threadView = new TaskView(ptr, this);

    QListWidgetItem* item = new QListWidgetItem(QString(), ui->tasksList);
    item->setSizeHint(threadView->minimumSizeHint());

    ui->tasksList->setItemWidget(item, threadView);
    ui->tasksList->addItem(item);
}

void TestApp::initConnections()
{
    // UI
    connect(ui->splitter, &QSplitter::splitterMoved, ui->splitter_2, [&] {
        ui->splitter_2->setSizes(ui->splitter->sizes());
        });
    connect(ui->splitter_2, &QSplitter::splitterMoved, ui->splitter, [&] {
        ui->splitter->setSizes(ui->splitter_2->sizes());
        });

    // Tasks
    connect(ui->tasksActions, &QToolButton::pressed, this, &TestApp::CreateTask);
}
