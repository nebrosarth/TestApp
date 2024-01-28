#include "TaskView.h"

TaskView::TaskView(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::TaskViewClass())
{
	ui->setupUi(this);
}

TaskView::~TaskView()
{
	delete ui;
}

void TaskView::setProgress(int value)
{
	ui->progressBar->setValue(value);
}

void TaskView::setText(const QString& text)
{
	ui->TaskTitle->setText(text);
}
