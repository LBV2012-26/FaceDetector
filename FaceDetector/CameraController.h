#pragma once

#include <QCloseEvent>
#include <QTimer>
#include <QtWidgets/QDialog>
#include <opencv2/videoio.hpp>

#include "ui_CameraController.h"

class CameraController : public QDialog {
	Q_OBJECT

public:
	CameraController(QWidget* Parent = nullptr);
	~CameraController();

public:
	void closeEvent(QCloseEvent* Event) override;

public:
	void OpenCamera();

signals:
	void Signal_DetectFace(QImage& Image);

private slots:
	void Slot_DrawFrame();
	void on_QPushButton_DetectFace_clicked();

private:
	Ui::CameraControllerClass* _Ui;
	cv::VideoCapture           _Capture;
	QTimer*					   _Timer;
	bool					   _bStartDetect;
};
