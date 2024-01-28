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

    void closeEvent(QCloseEvent* event) override;

    void createTask();
    void applyToSelectedTasks(std::function<void(Task*)> action);
    void pauseSelectedTasks();
    void resumeSelectedTasks();
    void cancelSelectedTasks();
    void removeTask(TaskView* taskView);
    void resumeAll();

protected:
    void initWidgets();
    void initConnections();
    void closeLater();

    QThreadPool& pool();
    std::unordered_map<TaskView*, std::unique_ptr<Task>>& tasks();

private:
    bool m_closeLater = false;
    Ui::TestAppClass *ui;
    QThreadPool m_pool;
    std::unordered_map<TaskView*, std::unique_ptr<Task>> m_tasks;
};
