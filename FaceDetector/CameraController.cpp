#include "CameraController.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <QImage>
#include <QMessageBox>

CameraController::CameraController(QWidget* Parent) : QDialog(Parent), _Ui(new Ui::CameraControllerClass), _bStartDetect(false) {
	_Timer = new QTimer(this);
	_Ui->setupUi(this);

	connect(_Timer, &QTimer::timeout, this, &CameraController::Slot_DrawFrame);
}

CameraController::~CameraController() {
	delete _Ui;
	_Ui = nullptr;

	if (_Timer) {
		delete _Timer;
		_Timer = nullptr;
	}
}

void CameraController::closeEvent(QCloseEvent* Event) {
	_Capture.release();
	_Timer->stop();
}

void CameraController::OpenCamera() {
	_Timer->start(1000.0 / 30.0);

	_Capture.open(0);
	if (!_Capture.isOpened()) {
		QMessageBox::information(this, "Error", "Failed to open camera");
		return;
	}

	_Capture.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
	_Capture.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
}

void CameraController::on_QPushButton_DetectFace_clicked() {
	_bStartDetect = !_bStartDetect;
}

void CameraController::Slot_DrawFrame() {
	cv::Mat Frame;
	_Capture >> Frame;

	QImage FrameImage = QImage(static_cast<const uchar*>(Frame.data), Frame.cols, Frame.rows, Frame.step, QImage::Format_RGB888);
	FrameImage = FrameImage.rgbSwapped();
	FrameImage = FrameImage.scaled(_Ui->QLabel_Camera->width(), _Ui->QLabel_Camera->height(),
		Qt::KeepAspectRatio, Qt::SmoothTransformation);

	if (_bStartDetect)
		Signal_DetectFace(FrameImage);

	_Ui->QLabel_Camera->setPixmap(QPixmap::fromImage(FrameImage));
}
