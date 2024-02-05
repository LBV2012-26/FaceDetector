#include "Detector.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <QCoreApplication>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QString>

Detector::Detector(QObject* Parent) : QObject(Parent) {
	if (!Init())
		return;
}

Detector::~Detector() {}

bool Detector::Init() {
	QString FaceCascadeModelFilename = "Resources/Models/haarcascade_frontalface_alt.xml";
	QString EyesCascadeModelFilename = "Resources/Models/haarcascade_eye.xml";

	if (!_FaceCascade.load(FaceCascadeModelFilename.toStdString()) ||
		!_EyesCascade.load(EyesCascadeModelFilename.toStdString()))
		return false;
	return true;
}

cv::Mat Detector::ImageToMatrix(const QImage& Image) {
	cv::Mat ImageMatrix;
	int     ImageFormat = 0;
	switch (Image.format()) {
	case QImage::Format_RGB32:
		ImageFormat = CV_8UC4;
		break;
	case QImage::Format_RGB888:
		ImageFormat = CV_8UC3;
		break;
	default:
		break;
	}

	ImageMatrix = cv::Mat(Image.height(), Image.width(), ImageFormat,
		static_cast<void*>(const_cast<uchar*>(Image.constBits())), Image.bytesPerLine());

	return ImageMatrix;
}

std::pair<std::vector<cv::Rect>, cv::Mat> Detector::FindFaces(const QImage& Image) {
	QImage  ImageCopy = Image.copy();
	cv::Mat Frame     = ImageToMatrix(ImageCopy);
	cv::Mat FrameGray;
	std::vector<cv::Rect> Faces;

	cv::cvtColor(Frame, FrameGray, CV_BGR2GRAY);
	cv::equalizeHist(FrameGray, FrameGray);

	_FaceCascade.detectMultiScale(FrameGray, Faces, 1.1, 6, 0);

	return { Faces, FrameGray };
}

std::vector<cv::Rect> Detector::FindEyes(const cv::Mat& FrameGray, const cv::Rect& Face) {
	std::vector<cv::Rect> Eyes;
	cv::Mat FaceRoi = FrameGray(Face);

	_EyesCascade.detectMultiScale(FaceRoi, Eyes, 1.1, 2, 0, cv::Size(20, 20));

	if (Eyes.size() != 2)
		Eyes.clear();
	else if (Face.height * 0.5 < Eyes[0].y || Face.height * 0.5 < Eyes[1].y)
		Eyes.clear();

	return Eyes;
}

void Detector::FrameFace(QImage& Image) {
	auto Result = FindFaces(Image);
	std::vector<cv::Rect> Faces = Result.first;
	cv::Mat FrameGray = Result.second;

	QPainter Painter(&Image);
	Painter.setPen(QPen(Qt::red, 4));

	QPoint CircleCenter;
	int    Radius = 0;

	for (auto& Face : Faces) {
		std::vector<cv::Rect> Eyes = FindEyes(FrameGray, Face);
		if (Eyes.size() == 2) {
			CircleCenter = QPoint(Face.x + Eyes[0].x + Eyes[0].width * 0.5,
				Face.y + Eyes[0].y + Eyes[0].height * 0.5);
			Radius = cvRound(Eyes[0].width * 0.25);
			Painter.drawEllipse(CircleCenter, Radius, Radius);

			CircleCenter = QPoint(Face.x + Eyes[1].x + Eyes[1].width * 0.5,
				Face.y + Eyes[1].y + Eyes[1].height * 0.5);
			Radius = cvRound(Eyes[1].width * 0.25);
			Painter.drawEllipse(CircleCenter, Radius, Radius);
		}

		Painter.drawRect(Face.x, Face.y, Face.width, Face.height);
	}
}
