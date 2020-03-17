#include "IntersectionGUI.h"
#include <QString>
#include <string>
#include <QListWidgetItem>
#include <fstream>
#include <iostream>
#include <QDir>
#include <QFileDialog>
#include <QPainter>
#include <vector>
#include <utility>
#include <QMessageBox>

#pragma comment(lib,"calcInterface.lib")
_declspec(dllexport) extern "C" int guiProcess(std::vector<std::pair<double, double>> *points, std::string msg);
_declspec(dllexport) extern "C" void cmdProcess(int argc, char *argv[]);

IntersectionGUI::IntersectionGUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	LabStatus = new QLabel("");
	ui.statusBar->addWidget(LabStatus);
	ui.tBtnAddFile->setDefaultAction(ui.actAddFile);
	ui.tBtnAddShape->setDefaultAction(ui.actAddShape);
	ui.tBtnDelShape->setDefaultAction(ui.actDelShape);
	ui.tBtnClear->setDefaultAction(ui.actClear);
	ui.tBtnSolve->setDefaultAction(ui.actSolve);
	ui.listWidget->clear();
	init_canvas();
}

void IntersectionGUI::on_actAddShape_triggered()
{
	QString shapeType = ui.shapeType->currentText();
	int data1 = ui.data1->value();
	int data2 = ui.data2->value();
	int data3 = ui.data3->value();
	int data4 = ui.data4->value();
	QString str;
	if (shapeType == QString::fromLocal8Bit("直线")) {
		str = str.sprintf("L %d %d %d %d",data1, data2, data3, data4);
		draw_line(data1, data2, data3, data4);
	}
	else if (shapeType == QString::fromLocal8Bit("线段")) {
		str = str.sprintf("S %d %d %d %d", data1, data2, data3, data4);
		draw_seg(data1, data2, data3, data4);
	}
	else if (shapeType == QString::fromLocal8Bit("射线")) {
		str = str.sprintf("R %d %d %d %d", data1, data2, data3, data4);
		draw_ray(data1, data2, data3, data4);
	}
	else {
		str =  str.sprintf("C %d %d %d", data1, data2, data3);
		draw_circle(data1, data2, data3);
	}
	QListWidgetItem * aItem = new QListWidgetItem(); //新建一个项
	aItem->setText(str); //设置文字标签
	aItem->setCheckState(Qt::Unchecked); //设置为选中状态
	aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
	ui.listWidget->addItem(aItem); //增加一个项
}

void IntersectionGUI::on_actDelShape_triggered() {
	for (int i = 0; i < ui.listWidget->count(); i++)
	{
		QListWidgetItem *aItem = ui.listWidget->item(i);//获取一个项
		if (aItem->checkState() == Qt::Checked) {
			ui.listWidget->takeItem(i);
			delete aItem;
			i--;
		}
	}
	init_canvas();
	// draw remained
	for (int i = 0; i < ui.listWidget->count(); i++)
	{
		QListWidgetItem *aItem = ui.listWidget->item(i);//获取一个项
		QString str = aItem->text();
		draw_shape_from_str(str);
	}
}

void IntersectionGUI::on_actClear_triggered() {
	ui.listWidget->clear();
	init_canvas();
}

void IntersectionGUI::on_actAddFile_triggered() {
	if (lastPath == QString::fromLocal8Bit("")) {
		lastPath = QDir::currentPath();//获取系统当前目录
	}
	//获取应用程序的路径
	QString dlgTitle = QString::fromLocal8Bit("选择一个文件"); //对话框标题
	QString filter = QString::fromLocal8Bit("文本文件(*.txt);;所有文件(*.*)"); //文件过滤器
	QString aFileName = QFileDialog::getOpenFileName(this, dlgTitle, lastPath, filter);
	if (!aFileName.isEmpty()) {
		lastPath = aFileName;
		std::ifstream fin(aFileName.toStdString());
		int N;
		std::string line;
		fin >> N;
		std::getline(fin, line);
		while (N--) {
			std::getline(fin, line);
			QString qline = QString::fromStdString(line);
			if (!isShapeStrValid(qline)) {
				QString dlgTitle = QString::fromLocal8Bit("输入格式错误");
				QMessageBox::critical(this, dlgTitle, qline);
				break;
			}
			draw_shape_from_str(qline);
			QListWidgetItem * aItem = new QListWidgetItem(); //新建一个项
			aItem->setText(qline); //设置文字标签
			aItem->setCheckState(Qt::Unchecked); //设置为选中状态
			aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
			ui.listWidget->addItem(aItem); //增加一个项
		}
	}
}

void IntersectionGUI::on_actSolve_triggered()
{
	std::string input;
	input += std::to_string(ui.listWidget->count()) + "\n";
	for (int i = 0; i < ui.listWidget->count(); i++)
	{
		QListWidgetItem *aItem = ui.listWidget->item(i);//获取一个项
		QString str = aItem->text();
		input += str.toStdString() + "\n";
	}
	std::vector<std::pair<double, double> > points;
	int cnt = guiProcess(&points, input);
	QString str = str.asprintf("%d %d %d", cnt, (int)(points.begin()->first), (int)(points.begin()->second));
	LabStatus->setText(str);
	for (auto vit = points.begin(); vit != points.end(); ++vit) {
		int w, h;
		xy2wh((int)(vit->first), (int)(vit->second), w, h);
		QString str = str.asprintf("%d %d %d", cnt, w, h);
		LabStatus->setText(str);
		draw_point(w, h, Qt::red);
	}
}

void IntersectionGUI::draw_point(int const x, int const y, QColor const c, int const w) {
	// 在QPixmap上画图
	QPainter Painter(&curPixmap);
	Painter.setRenderHint(QPainter::Antialiasing, true);

	Painter.setPen(QPen(c, w));
	QBrush brush(c);
	Painter.setBrush(brush);
	Painter.drawEllipse(x - w, y - w , w * 2, w * 2);
	ui.canvas->setPixmap(curPixmap);
}

void IntersectionGUI::draw_axis()
{
	QPainter Painter(&curPixmap);
	Painter.setPen(QPen(Qt::green, 2));
	Painter.drawLine(QPoint(0, homeH), QPoint(CANVAS_SIZE, homeH));
	Painter.drawLine(QPoint(homeW, CANVAS_SIZE), QPoint(homeW , 0));
}

void IntersectionGUI::init_canvas()
{
	curPixmap = QPixmap(CANVAS_SIZE, CANVAS_SIZE);
	curPixmap.fill(Qt::white);
	draw_axis();
	ui.canvas->setPixmap(curPixmap);
}

void IntersectionGUI::draw_line(int x1, int y1, int x2, int y2, QColor const c, int const w)
{
	IntersectionGUI::draw_ray(x1, y1, x2, y2, c, w);
	IntersectionGUI::draw_ray(x2, y2, x1, y1, c, w);
}

void IntersectionGUI::draw_seg(int x1, int y1, int x2, int y2, QColor const c, int const w)
{
	QPainter Painter(&curPixmap);
	Painter.setRenderHint(QPainter::Antialiasing, true);
	Painter.setPen(QPen(c, w));
	QPoint p1 = xy2whPoint(x1, y1);
	QPoint p2 = xy2whPoint(x2, y2);
	QString str = str.asprintf("%d %d %d %d %d %d %d %d", p1.x(), p1.y(), p2.x(), p2.y(), x1, y1, x2, y2);
	LabStatus->setText(str);
	Painter.drawLine(p1, p2);
	ui.canvas->setPixmap(curPixmap);
}

void IntersectionGUI::draw_ray(int x1, int y1, int x2, int y2, QColor const c, int const w) {
	QPainter Painter(&curPixmap);
	Painter.setRenderHint(QPainter::Antialiasing, true);
	Painter.setPen(QPen(c, w));
	if (x2 == x1) {
		if (y2 > y1) {
			y2 = REAL_SIZE;
		}
		else {
			y2 = -REAL_SIZE;
		}
	}
	else if (y1 == y2) {
		if (x2 > x1) {
			x2 = REAL_SIZE;
		}
		else {
			x2 = -REAL_SIZE;
		}
	}
	else {
		double k = (double)(y2 - y1) / (x2 - x1);
		double b = y1 - k * x1;
		if (x2 > x1) {
			double y_ = REAL_SIZE * k + b;
			if (y_ > REAL_SIZE) {
				y2 = REAL_SIZE;
				x2 = (y2 - b) / k;
			}
			else if (y_ < -REAL_SIZE) {
				y2 = -REAL_SIZE;
				x2 = (y2 - b) / k;
			}
			else {
				x2 = REAL_SIZE;
				y2 = y_;
			}
		}
		else {
			double y_ = -REAL_SIZE * k + b;
			if (y_ > REAL_SIZE) {
				y2 = REAL_SIZE;
				x2 = (y2 - b) / k;
			}
			else if (y_ < -REAL_SIZE) {
				y2 = -REAL_SIZE;
				x2 = (y2 - b) / k;
			}
			else {
				x2 = -REAL_SIZE;
				y2 = y_;
			}
		}
	}
	QPoint p1 = xy2whPoint(x1, y1);
	QPoint p2 = xy2whPoint(x2, y2);
	QString str = str.asprintf("%d %d %d %d %d %d %d %d", p1.x(), p1.y(), p2.x(), p2.y(), x1, y1, x2, y2);
	LabStatus->setText(str);
	Painter.drawLine(p1, p2);
	ui.canvas->setPixmap(curPixmap);
}

void IntersectionGUI::draw_circle(int x0, int y0, int r, QColor const c, int const w)
{
	QPainter Painter(&curPixmap);
	Painter.setRenderHint(QPainter::Antialiasing, true);
	Painter.setPen(QPen(c, w));
	QPoint p0 = xy2whPoint(x0, y0);
	int r0 = double(r)* RATIO;
	QString str = str.asprintf("%d %d %d", p0.x() - r0, p0.y() - r0, r0 * 2);
	LabStatus->setText(str);
	Painter.drawEllipse(p0.x() - r0, p0.y() - r0, r0 * 2, r0 * 2 );
	ui.canvas->setPixmap(curPixmap);
}

void IntersectionGUI::xy2wh(int x, int y, int & w, int & h)
{
	w = (double)x * RATIO + homeW;
	h = homeH - (double)y * RATIO;
}

QPoint IntersectionGUI::xy2whPoint(int x, int y)
{
	int w, h;
	xy2wh(x, y, w, h);
	return QPoint(w,h);
}

void IntersectionGUI::draw_shape_from_str(QString str) {
	QString shapeType = str.section(" ", 0, 0);
	if (shapeType == QString::fromLocal8Bit("L")) {
		int data1 = str.section(" ", 1, 1).toInt();
		int data2 = str.section(" ", 2, 2).toInt();
		int data3 = str.section(" ", 3, 3).toInt();
		int data4 = str.section(" ", 4, 4).toInt();
		draw_line(data1, data2, data3, data4);
	}
	else if (shapeType == QString::fromLocal8Bit("S")) {
		int data1 = str.section(" ", 1, 1).toInt();
		int data2 = str.section(" ", 2, 2).toInt();
		int data3 = str.section(" ", 3, 3).toInt();
		int data4 = str.section(" ", 4, 4).toInt();
		draw_seg(data1, data2, data3, data4);
	}
	else if (shapeType == QString::fromLocal8Bit("R")) {
		int data1 = str.section(" ", 1, 1).toInt();
		int data2 = str.section(" ", 2, 2).toInt();
		int data3 = str.section(" ", 3, 3).toInt();
		int data4 = str.section(" ", 4, 4).toInt();
		draw_ray(data1, data2, data3, data4);
	}
	else {
		int data1 = str.section(" ", 1, 1).toInt();
		int data2 = str.section(" ", 2, 2).toInt();
		int data3 = str.section(" ", 3, 3).toInt();
		draw_circle(data1, data2, data3);
	}
}

bool IntersectionGUI::isShapeStrValid(QString str)
{
	str = str.trimmed();
	QRegExp re("([LRS]( (-|\\+)?\\d+){4})|(C( (-|\\+)?\\d+){3})");
	if (!re.exactMatch(str)) {
		return false;
	}
	return true;
}

