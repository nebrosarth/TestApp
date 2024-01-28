#pragma once
#include <qrunnable.h>
#include <qobject.h>

class Task : public QObject, public QRunnable
{
	Q_OBJECT
public:
	Task(int numIters = 1); // Number of iterations to complete
	void run() override;
	bool isPaused() const;
	bool isCanceled() const;
	void pause();
	void resume();
	void cancel();
signals:
	void finished();
	void progress(int value);

protected:
	void processPause(); // Check if paused and wait

protected:
	bool m_paused = false;
	bool m_canceled = false;

	int m_numIterations = 0;
};

