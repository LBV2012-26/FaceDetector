#pragma once

#include <opencv2/objdetect.hpp>
#include <QImage>
#include <QObject>
#include <utility>
#include <vector>

class Detector : public QObject {
	Q_OBJECT

public:
	Detector(QObject* Parent = nullptr);
	~Detector();

public:
	void FrameFace(QImage& Image);

private:
	bool Init();
	cv::Mat ImageToMatrix(const QImage& Image);
	std::pair<std::vector<cv::Rect>, cv::Mat> FindFaces(const QImage& Image);
	std::vector<cv::Rect> FindEyes(const cv::Mat& FrameGray, const cv::Rect& Face);

private:
	cv::CascadeClassifier _EyesCascade;
	cv::CascadeClassifier _FaceCascade;
};
