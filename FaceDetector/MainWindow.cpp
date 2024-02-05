#include "MainWindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QPixmap>

MainWindow::MainWindow(QWidget* Parent) : QDialog(Parent), _Ui(new Ui::MainWindowClass) {
	_Ui->setupUi(this);
}

MainWindow::~MainWindow() {
	delete _Ui;
	_Ui = nullptr;
}

void MainWindow::on_QPushButton_Detect_clicked() {
	if (_ImageFilename.isEmpty())
		return;

	QImage Image(_ImageFilename, "QImage::Format_RGB888");
	Image = Image.scaled(_Ui->QLabel_Photo->width(), _Ui->QLabel_Photo->height(),
		Qt::KeepAspectRatio, Qt::SmoothTransformation);
	Q_EMIT Signal_DetectFace(Image);

	_Ui->QLabel_Photo->setPixmap(QPixmap::fromImage(Image));
}

void MainWindow::on_QPushButton_OpenCamera_clicked() {
	Q_EMIT Signal_OpenCamera();
}

void MainWindow::on_QPushButton_SampleData_clicked() {
	qDebug() << __func__;
}

void MainWindow::on_QPushButton_Select_clicked() {
	_ImageFilename = QFileDialog::getOpenFileName(this, "Open photo", ".",
		"图像 (*.jpg *.jpeg *.png *bmp *tif *tiff)");
	if (_ImageFilename.isEmpty())
		return;

	QImage Image(_ImageFilename, "QImage::Format_RGB888");
	Image = Image.scaled(_Ui->QLabel_Photo->width(), _Ui->QLabel_Photo->height(),
		Qt::KeepAspectRatio, Qt::SmoothTransformation);
	_Ui->QLabel_Photo->setPixmap(QPixmap::fromImage(Image));
}
