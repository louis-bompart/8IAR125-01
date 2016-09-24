#include "myclass.h"

MyClass::MyClass(QWidget *parent)
	: QMainWindow(parent)
{
	main_layout = new QHBoxLayout();

	initializeStates();
	initializeMessages();

	main_widget = new QWidget();
	main_widget->setLayout(main_layout);
	setCentralWidget(main_widget);
}

MyClass::~MyClass()
{

}

void MyClass::initializeStates() {
	m_thirst_level = new QLabel("0");
	m_location = new QLabel("Lalala");
	m_location->setText("Miner : home");

	main_layout->addWidget(m_thirst_level);
	main_layout->addWidget(m_location);
}

void MyClass::initializeMessages() {
	messages_show = new QPlainTextEdit();

	messages_show->appendHtml("<p style=\"color:red\">Welcome to the telegram system</p>");
	messages_show->setEnabled(false);
	messages_show->appendHtml("<p style=\"color:red\">This is where Bob, Louis and Elsa communicates</p>");

	main_layout->addWidget(messages_show);
}