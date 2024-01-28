#pragma once

#include <QtWidgets/QWidget>
#include "ui_TestApp.h"
#include <qthreadpool.h>
#include "TaskView.h"
#include "Task.h"
#include "qfuture.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TestAppClass; };
QT_END_NAMESPACE

class TaskView;

class TestApp : public QWidget
{
    Q_OBJECT

public:
    TestApp(QWidget *parent = nullptr);
    ~TestApp();
    void CreateTask();
    void PauseSelectedTasks();
    void ResumeSelectedTasks();
    void CancelSelectedTasks();
    void RemoveTask(TaskView* taskView);

protected:
    void initConnections();

private:
    Ui::TestAppClass *ui;
    QThreadPool m_pool;
    std::unordered_map<TaskView*, std::shared_ptr<Task>> m_tasks;
};
