#pragma once

#include <QWidget>
#include "ui_TaskView.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TaskViewClass; };
QT_END_NAMESPACE

class Task;

class TaskView : public QWidget
{
	Q_OBJECT

public:
	TaskView(std::shared_ptr<Task> thread, QWidget* parent = nullptr);
	~TaskView();

private:
	Ui::TaskViewClass *ui;
	std::shared_ptr<Task> m_thread;
};
