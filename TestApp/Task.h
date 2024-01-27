#pragma once
#include <qrunnable.h>
class Task : public QRunnable
{
public:
	Task() {
		int x = 1;
	}
	~Task() {
		int x = 1;
	}
	void run() override;
	bool isPaused() const;
	void pause();
	void resume();

protected:
	bool m_paused = false;
};

