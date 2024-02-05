#pragma once

#include <QImage>
#include <QObject>

#include "CameraController.h"
#include "Detector.h"
#include "MainWindow.h"

class Kernel : public QObject {
	Q_OBJECT

public:
	Kernel(QObject* Parent = nullptr);
	~Kernel();

private slots:
	void Slot_DetectFace(QImage& Image);
	void Slot_OpenCamera();

private:
	CameraController* _CameraWindow;
	Detector*         _Detector;
	MainWindow*       _MainWindow;
};
