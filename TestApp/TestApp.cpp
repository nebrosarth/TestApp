#include "TestApp.h"
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
	for (const auto& tasks : tasks()) {
		tasks.second->cancel(); // Ask tasks to finish job
	}

	pool().waitForDone(); // Wait all threads to finish

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
		if (pool().activeThreadCount() > 0) {

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

void TestApp::createTask()
{
	static long taskId = 0;
	const int taskIterations = 100;

	auto taskPair = tasks().emplace(new TaskView(this), std::make_unique<Task>(taskIterations));
	if (taskPair.second) /*Check inserted*/ {
		QListWidgetItem* item = new QListWidgetItem(QString(), ui->tasksList);

		TaskView* taskView = taskPair.first->first;
		taskView->setText("Task " + QString::number(++taskId));

		item->setSizeHint(taskView->minimumSizeHint());
		ui->tasksList->setItemWidget(item, taskView);
		ui->tasksList->addItem(item);

		Task* task = taskPair.first->second.get();

		pool().start(task); // Start task

		connect(task, &Task::progress, taskView, &TaskView::setProgress);
		connect(task, &Task::finished, this, [=]() {
			removeTask(taskView);
			closeLater();
			});
	}
}

void TestApp::applyToSelectedTasks(std::function<void(Task*)> action)
{
	QListWidget* tasksList = ui->tasksList;
	QList<QListWidgetItem*> selected = tasksList->selectedItems();
	for (auto item : selected) {
		TaskView* taskView = dynamic_cast<TaskView*>(tasksList->itemWidget(item));
		if (taskView) {
			if (auto taskPair = tasks().find(taskView); taskPair != tasks().end()) {
				action(taskPair->second.get());
			}
		}
	}
}

void TestApp::pauseSelectedTasks()
{
	applyToSelectedTasks([](Task* task) {
		task->pause();
		});
}

void TestApp::resumeSelectedTasks()
{
	applyToSelectedTasks([](Task* task) {
		task->resume();
		});
}

void TestApp::cancelSelectedTasks()
{
	applyToSelectedTasks([](Task* task) {
		task->cancel();
		});
}

void TestApp::removeTask(TaskView* taskView)
{
	QListWidget* const tasksList = ui->tasksList;
	for (int i = 0; i < tasksList->count(); ++i) {
		QListWidgetItem* item = tasksList->item(i);
		QWidget* itemWidget = tasksList->itemWidget(item);
		if (dynamic_cast<TaskView*>(itemWidget) == taskView) {
			tasks().erase(taskView);
			delete item;
			itemWidget->deleteLater();
		}
	}
}

void TestApp::resumeAll()
{
	for (const auto& taskPair : tasks()) {
		taskPair.second->resume();
	}
}

void TestApp::initWidgets()
{
	ui->splitter->setSizes(QList<int>({ INT_MAX, INT_MAX }));
	ui->splitter_2->setSizes(QList<int>({ INT_MAX, INT_MAX }));

	ui->textEdit->setText("Qt is used for developing graphical user interfaces (GUIs) and multi-platform applications that run on all major desktop platforms and mobile or embedded platforms. \nMost GUI programs created with Qt have a native-looking interface, in which case Qt is classified as a widget toolkit. Non-GUI programs can also be developed, such as command-line tools and consoles for servers. \nAn example of such a non-GUI program using Qt is the Cutelyst web framework. \nQt supports various compilers, including the GCC and Clang C++ compilers, the Visual Studio suite, Python via Python bindings, PHP via an extension for PHP5, and has extensive internationalization support. Qt also provides Qt Quick, that includes a declarative scripting language called QML that allows using JavaScript to provide the logic. \nWith Qt Quick, rapid application development for mobile devices became possible, while logic can still be written with native code as well to achieve the best possible performance. \nOther features include SQL database access, XML parsing, JSON parsing, thread management and network support.");

	QListWidget* listWidget1 = new QListWidget(this);
	QVBoxLayout* layout1 = new QVBoxLayout(this);
	layout1->addWidget(listWidget1);

	// Section 1
	ui::Section* properties = new ui::Section("Properties", 300, this);
	properties->setContentLayout(*layout1);

	ui->filtersLayout->addWidget(properties);

	// Section 2
	QListWidget* listWidget2 = new QListWidget(this);
	QVBoxLayout* layout2 = new QVBoxLayout(this);
	layout2->addWidget(listWidget2);

	ui::Section* filtersAndSearch = new ui::Section("Filters and search", 300, this);
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

	// Tasks
	connect(ui->createTaskButton, &QToolButton::pressed, this, &TestApp::createTask);
	connect(ui->resumeTaskButton, &QToolButton::pressed, this, &TestApp::resumeSelectedTasks);
	connect(ui->pauseTaskButton, &QToolButton::pressed, this, &TestApp::pauseSelectedTasks);
	connect(ui->cancelTaskButton, &QToolButton::pressed, this, &TestApp::cancelSelectedTasks);
}

void TestApp::closeLater()
{
	if (m_closeLater) {
		if (pool().activeThreadCount() == 0) {
			close();
		}
	}
}

QThreadPool& TestApp::pool()
{
	return m_pool;
}

std::unordered_map<TaskView*, std::unique_ptr<Task>>& TestApp::tasks()
{
	return m_tasks;
}
