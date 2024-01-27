#include "TaskView.h"

TaskView::TaskView(std::shared_ptr<Task> thread, QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::TaskViewClass()), m_thread(thread)
{
	ui->setupUi(this);
}

TaskView::~TaskView()
{
	delete ui;
}
