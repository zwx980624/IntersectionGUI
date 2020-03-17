#include "IntersectionGUI.h"
#include <QString>
#include <string>
#include <QListWidgetItem>
#include <fstream>
#include <iostream>
#include <QDir>
#include <QFileDialog>
#include <QPainter>



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
	curPixmap = QPixmap(CANVAS_SIZE, CANVAS_SIZE);
	curPixmap.fill(Qt::white);
	draw_axis();
	//draw_point(500, 500, QColor(10, 20, 20), 10);
	ui.canvas->setPixmap(curPixmap);
}

void IntersectionGUI::on_actAddShape_triggered()
{
	QString shapeType = ui.shapeType->currentText();
	int data1 = ui.data1->value();
	int data2 = ui.data2->value();
	int data3 = ui.data3->value();
	int data4 = ui.data4->value();
	QString str;
	if (shapeType == QString::fromLocal8Bit("ֱ��")) {
		str = str.sprintf("L %d %d %d %d",data1, data2, data3, data4);
	}
	else if (shapeType == QString::fromLocal8Bit("�߶�")) {
		str = str.sprintf("S %d %d %d %d", data1, data2, data3, data4);
		draw_seg(data1, data2, data3, data4);
	}
	else if (shapeType == QString::fromLocal8Bit("����")) {
		str = str.sprintf("R %d %d %d %d", data1, data2, data3, data4);
	}
	else {
		str =  str.sprintf("C %d %d %d", data1, data2, data3);
	}
	QListWidgetItem * aItem = new QListWidgetItem(); //�½�һ����
	aItem->setText(str); //�������ֱ�ǩ
	aItem->setCheckState(Qt::Unchecked); //����Ϊѡ��״̬
	aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
	ui.listWidget->addItem(aItem); //����һ����
}

void IntersectionGUI::on_actDelShape_triggered() {
	for (int i = 0; i < ui.listWidget->count(); i++)
	{
		QListWidgetItem *aItem = ui.listWidget->item(i);//��ȡһ����
		if (aItem->checkState() == Qt::Checked) {
			ui.listWidget->takeItem(i);
			delete aItem;
			i--;
		}
	}
}

void IntersectionGUI::on_actClear_triggered() {
	ui.listWidget->clear();
}

void IntersectionGUI::on_actAddFile_triggered() {
	if (lastPath == QString::fromLocal8Bit("")) {
		lastPath = QDir::currentPath();//��ȡϵͳ��ǰĿ¼
	}
	//��ȡӦ�ó����·��
	QString dlgTitle = QString::fromLocal8Bit("ѡ��һ���ļ�"); //�Ի������
	QString filter = QString::fromLocal8Bit("�ı��ļ�(*.txt);;�����ļ�(*.*)"); //�ļ�������
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
			QListWidgetItem * aItem = new QListWidgetItem(); //�½�һ����
			aItem->setText(qline); //�������ֱ�ǩ
			aItem->setCheckState(Qt::Unchecked); //����Ϊѡ��״̬
			aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
			ui.listWidget->addItem(aItem); //����һ����
		}
	}
}

void IntersectionGUI::draw_point(int const x, int const y, QColor const c, int const w) {
	// ��QPixmap�ϻ�ͼ
	QPainter Painter(&curPixmap);

	Painter.setPen(QPen(c, w));

	Painter.drawPoint(x, y);
}

void IntersectionGUI::draw_axis()
{
	QPainter Painter(&curPixmap);
	Painter.setPen(QPen(Qt::black, 2));
	Painter.drawLine(QPoint(0, homeH), QPoint(CANVAS_SIZE, homeH));
	Painter.drawLine(QPoint(homeW, CANVAS_SIZE), QPoint(homeW , 0));
}

void IntersectionGUI::draw_line(int x1, int y1, int x2, int y2, QColor const c, int const w)
{
	
	QPainter Painter(&curPixmap);
	Painter.setPen(QPen(c, w));
	Painter.drawLine(QPoint(0, homeH), QPoint(CANVAS_SIZE, homeH));
	Painter.drawLine(QPoint(homeW, CANVAS_SIZE), QPoint(homeW, 0));
}

void IntersectionGUI::draw_seg(int x1, int y1, int x2, int y2, QColor const c, int const w)
{
	QPainter Painter(&curPixmap);
	Painter.setPen(QPen(c, w));
	QPoint p1 = xy2whPoint(x1, y1);
	QPoint p2 = xy2whPoint(x2, y2);
	QString str = str.asprintf("%d %d %d %d %d %d %d %d", p1.x(), p1.y(), p2.x(), p2.y(), x1, y1, x2, y2);
	LabStatus->setText(str);
	Painter.drawLine(p1, p2);
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

