#include "IntersectionGUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	IntersectionGUI w;
	w.show();
	return a.exec();
}
