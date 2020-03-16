#include "IntersectionGUI.h"
#include <QString>
#include <string>
#include <QListWidgetItem>
#include <fstream>
#include <iostream>
#include <QDir>
#include <QFileDialog>

IntersectionGUI::IntersectionGUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.tBtnAddFile->setDefaultAction(ui.actAddFile);
	ui.tBtnAddShape->setDefaultAction(ui.actAddShape);
	ui.tBtnDelShape->setDefaultAction(ui.actDelShape);
	ui.tBtnClear->setDefaultAction(ui.actClear);
	ui.tBtnSolve->setDefaultAction(ui.actSolve);
	ui.listWidget->clear();
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