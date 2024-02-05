#include "Kernel.h"

Kernel::Kernel(QObject* Parent) : QObject(Parent) {
	_CameraWindow = new CameraController;
	_Detector     = new Detector;
	_MainWindow   = new MainWindow;

	_MainWindow->show();

	connect(_MainWindow,   &MainWindow::Signal_DetectFace,       this, &Kernel::Slot_DetectFace);
	connect(_MainWindow,   &MainWindow::Signal_OpenCamera,       this, &Kernel::Slot_OpenCamera);
	connect(_CameraWindow, &CameraController::Signal_DetectFace, this, &Kernel::Slot_DetectFace);
}

Kernel::~Kernel() {
	if (_CameraWindow) {
		_CameraWindow->hide();
		delete _CameraWindow;
		_CameraWindow = nullptr;
	}

	if (_Detector) {
		delete _Detector;
		_Detector = nullptr;
	}

	if (_MainWindow) {
		_MainWindow->hide();
		delete _MainWindow;
		_MainWindow = nullptr;
	}
}

void Kernel::Slot_OpenCamera() {
	_CameraWindow->show();
	_CameraWindow->OpenCamera();
}

void Kernel::Slot_DetectFace(QImage& Image) {
	_Detector->FrameFace(Image);
}
