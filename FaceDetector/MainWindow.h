#pragma once

#include <QImage>
#include <QString>
#include <QtWidgets/QDialog>

#include "ui_MainWindow.h"

class MainWindow : public QDialog {
	Q_OBJECT

public:
	MainWindow(QWidget* Parent = nullptr);
	~MainWindow();

signals:
	void Signal_DetectFace(QImage& Image);
	void Signal_OpenCamera();

private slots:
	void on_QPushButton_Detect_clicked();
	void on_QPushButton_OpenCamera_clicked();
	void on_QPushButton_SampleData_clicked();
	void on_QPushButton_Select_clicked();

private:
	Ui::MainWindowClass* _Ui;
	QString _ImageFilename;
};
