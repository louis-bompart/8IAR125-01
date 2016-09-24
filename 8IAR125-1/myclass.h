#ifndef MYCLASS_H
#define MYCLASS_H

#include <QtWidgets/QMainWindow>
#include "ui_myclass.h"

class MyClass : public QMainWindow
{
	Q_OBJECT

public:
	MyClass(QWidget *parent = 0);
	~MyClass();

private:
	void initializeInterface();
	void initializeStates();
	void initializeMessages();

private:
	QWidget *main_widget;
	QWidget *states;
	QWidget *messages;

	QLayout *main_layout;

	QLabel *m_thirst_level;
	QLabel *m_location;

	QPlainTextEdit *messages_show;
};

#endif // MYCLASS_H
