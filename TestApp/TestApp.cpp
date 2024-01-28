#include "TestApp.h"
#include <QtConcurrent>
#include <qevent.h>
#include <QMessageBox>
#include "Section/Section.h"

TestApp::TestApp(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::TestAppClass())
{
	ui->setupUi(this);

	initWidgets();
	initConnections();
}

TestApp::~TestApp()
{
	for (auto tasks : m_tasks) {
		tasks.second->cancel(); // Ask tasks to finish job
	}

	m_pool.waitForDone(); // Wait all threads to finish

	delete ui;
}

void TestApp::closeEvent(QCloseEvent* event)
{
	if (m_closeLater) {
		return; // Just close it
	}

	int result = QMessageBox::question(this, "Close Dialog", "Close application?", "Close", "Wait tasks and then exit", "Cancel", 2);
	switch (result) {
	case 0: {
		event->accept();
		break;
	}
	case 1: {
		this->setEnabled(false);
		if (m_pool.activeThreadCount() > 0) {

			resumeAll(); // anti-hardlock

			m_closeLater = true;
			event->ignore();
		}
		break;
	}
	case 2: {
		event->ignore();
		break;
	}
	default: {

	}
	}
}

void TestApp::CreateTask()
{
	static long taskId = 0;
	const int taskIterations = 100;

	auto taskPair = m_tasks.emplace(new TaskView(this), std::make_shared<Task>(taskIterations));
	if (taskPair.second) /*Check inserted*/ {
		QListWidgetItem* item = new QListWidgetItem(QString(), ui->tasksList);

		TaskView* taskView = taskPair.first->first;
		taskView->setText("Task " + QString::number(++taskId));

		Task* task = taskPair.first->second.get();

		item->setSizeHint(taskView->minimumSizeHint());

		ui->tasksList->setItemWidget(item, taskView);
		ui->tasksList->addItem(item);

		m_pool.start(task);

		connect(task, &Task::progress, taskView, &TaskView::setProgress);
		connect(task, &Task::finished, this, [=]() {
			RemoveTask(taskView);
			CloseLater();
			});
	}
}

void TestApp::PauseSelectedTasks()
{
	QListWidget* tasksList = ui->tasksList;
	QList<QListWidgetItem*> selected = tasksList->selectedItems();
	for (auto item : selected) {
		TaskView* taskView = dynamic_cast<TaskView*>(tasksList->itemWidget(item));
		if (taskView) {
			if (auto taskPair = m_tasks.find(taskView); taskPair != m_tasks.end()) {
				taskPair->second->pause();
			}
		}
	}
}

void TestApp::ResumeSelectedTasks()
{
	QListWidget* tasksList = ui->tasksList;
	QList<QListWidgetItem*> selected = tasksList->selectedItems();
	for (auto item : selected) {
		TaskView* taskView = dynamic_cast<TaskView*>(tasksList->itemWidget(item));
		if (taskView) {
			if (auto taskPair = m_tasks.find(taskView); taskPair != m_tasks.end()) {
				taskPair->second->resume();
			}
		}
	}
}

void TestApp::CancelSelectedTasks()
{
	QListWidget* tasksList = ui->tasksList;
	QList<QListWidgetItem*> selected = tasksList->selectedItems();
	for (auto item : selected) {
		TaskView* taskView = dynamic_cast<TaskView*>(tasksList->itemWidget(item));
		if (taskView) {
			if (auto taskPair = m_tasks.find(taskView); taskPair != m_tasks.end()) {
				taskPair->second->cancel();
			}
		}
	}
}

void TestApp::RemoveTask(TaskView* taskView)
{
	QListWidget* const tasksList = ui->tasksList;
	for (int i = 0; i < tasksList->count(); ++i) {
		QListWidgetItem* item = tasksList->item(i);
		QWidget* itemWidget = tasksList->itemWidget(item);
		if (dynamic_cast<TaskView*>(itemWidget) == taskView) {
			m_tasks.erase(taskView);
			delete item;
			itemWidget->deleteLater();
		}
	}
}

void TestApp::resumeAll()
{
	for (auto taskPair : m_tasks) {
		taskPair.second->resume();
	}
}

void TestApp::initWidgets()
{
	ui->splitter->setSizes(QList<int>({ INT_MAX, INT_MAX }));
	ui->splitter_2->setSizes(QList<int>({ INT_MAX, INT_MAX }));


	QListWidget* listWidget1 = new QListWidget(this);
	QVBoxLayout* layout1 = new QVBoxLayout(this);
	layout1->addWidget(listWidget1);

	// Section 1
	ui::Section* properties = new ui::Section(u8"Свойства", 300, this);
	properties->setContentLayout(*layout1);

	ui->filtersLayout->addWidget(properties);

	// Section 2
	QListWidget* listWidget2 = new QListWidget(this);
	QVBoxLayout* layout2 = new QVBoxLayout(this);
	layout2->addWidget(listWidget2);

	ui::Section* filtersAndSearch = new ui::Section(u8"Фильтры и поиск", 300, this);
	filtersAndSearch->setContentLayout(*layout2);

	ui->filtersLayout->addWidget(filtersAndSearch);

	QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
	ui->filtersLayout->addSpacerItem(spacer);
}

void TestApp::initConnections()
{
	// UI
	connect(ui->splitter, &QSplitter::splitterMoved, ui->splitter_2, [&]() {
		ui->splitter_2->setSizes(ui->splitter->sizes());
		});
	connect(ui->splitter_2, &QSplitter::splitterMoved, ui->splitter, [&]() {
		ui->splitter->setSizes(ui->splitter_2->sizes());
		});
	connect(ui->tasksList, &QListWidget::itemClicked, this, [&](QListWidgetItem* item) {
		auto res = m_tasks.find((TaskView*)ui->tasksList->itemWidget(item));
		if (res != m_tasks.end()) {
			int x = 1;
		}
		});

	// Tasks
	connect(ui->createTaskButton, &QToolButton::pressed, this, &TestApp::CreateTask);
	connect(ui->resumeTaskButton, &QToolButton::pressed, this, &TestApp::ResumeSelectedTasks);
	connect(ui->pauseTaskButton, &QToolButton::pressed, this, &TestApp::PauseSelectedTasks);
	connect(ui->cancelTaskButton, &QToolButton::pressed, this, &TestApp::CancelSelectedTasks);
}

void TestApp::CloseLater()
{
	if (m_closeLater) {
		if (m_pool.activeThreadCount() == 0) {
			close();
		}
	}
}
