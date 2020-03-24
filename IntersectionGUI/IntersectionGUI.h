#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_IntersectionGUI.h"
#include <QString>
#include <QPixmap>

#define CANVAS_SIZE 1000
#define REAL_SIZE  200000
#define RATIO CANVAS_SIZE / REAL_SIZE

class IntersectionGUI : public QMainWindow
{
	Q_OBJECT

public:
	IntersectionGUI(QWidget *parent = Q_NULLPTR);

private:
	Ui::IntersectionGUIClass ui;
	QString lastPath;
	QPixmap curPixmap; //µ±Ç°µÄÍ¼Æ¬
	void draw_point(int const x, int const y, QColor const c, int const w = 4);
	int homeW = CANVAS_SIZE / 2;
	int homeH = CANVAS_SIZE / 2;
	void draw_axis();
	void init_canvas();
	void draw_line(int x1, int y1, int x2, int y2, QColor const c = Qt::black, int const w = 2);
	void draw_seg(int x1, int y1, int x2, int y2, QColor const c = Qt::black, int const w = 2);
	void draw_ray(int x1, int y1, int x2, int y2, QColor const c = Qt::black, int const w = 2);
	void draw_circle(int x0, int y0, int r, QColor const c = Qt::black, int const w = 2);
	void xy2wh(int x, int y, int& w, int& h);
	void draw_shape_from_str(QString str);
	bool isShapeStrValid(QString str);
	QPoint xy2whPoint(int x, int y);
	QLabel *LabStatus;
private slots:
	void on_actAddShape_triggered();
	void on_actDelShape_triggered();
	void on_actClear_triggered();
	void on_actAddFile_triggered();
	void on_actSolve_triggered();
	void on_shapeType_currentIndexChanged();
	void on_actAbout_triggered();
	void on_actSample1_triggered();
	void on_actSample2_triggered();
};


