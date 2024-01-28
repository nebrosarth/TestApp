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
	TaskView(QWidget* parent = nullptr);
	~TaskView();
	void setProgress(int value);
	void setText(const QString& text);

private:
	std::unique_ptr<Ui::TaskViewClass> ui;
};
