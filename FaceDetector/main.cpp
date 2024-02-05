#include <QtWidgets/QApplication>
#include "Kernel.h"

int main(int argc, char** argv) {
	QApplication App(argc, argv);
	Kernel       MainWindow;

	return App.exec();
}
