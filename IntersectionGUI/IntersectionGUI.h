#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_IntersectionGUI.h"
#include <QString>

class IntersectionGUI : public QMainWindow
{
	Q_OBJECT

public:
	IntersectionGUI(QWidget *parent = Q_NULLPTR);

private:
	Ui::IntersectionGUIClass ui;
	QString lastPath;

private slots:
	void on_actAddShape_triggered();
	void on_actDelShape_triggered();
	void on_actClear_triggered();
	void on_actAddFile_triggered();
};
