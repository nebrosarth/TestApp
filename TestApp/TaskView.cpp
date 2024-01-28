#include "TaskView.h"

TaskView::TaskView(QWidget* parent)
	: QWidget(parent)
	, ui(std::make_unique<Ui::TaskViewClass>())
{
	ui->setupUi(this);
}

TaskView::~TaskView()
{
}

void TaskView::setProgress(int value)
{
	ui->progressBar->setValue(value);
}

void TaskView::setText(const QString& text)
{
	ui->TaskTitle->setText(text);
}
