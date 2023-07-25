#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <string.h>
#include <vector>
#include <fstream>


using namespace std;
using namespace cv;
using namespace dnn;

class Detection
{
	public:
		Detection();
		~Detection();

		void Initialize(int width,int height);
		void ReadModel();
		bool Detecting(Mat frame);
		bool Afterure(Mat frame);
		vector<string> GetOutsNames();
		void Postprocess();
		void Drawer();
		void DrawBoxes(int classId,float conf,int left,int top,int right, int bottom);
		Mat GetFrame();
		int GetResWidth();
		int GetResHeight();
		void Dump();

	private:

		int m_width;
		int m_height;

		dnn::Net m_model;
		Mat m_frame;
		Mat m_blob;
		vector<Mat> m_outs;
		vector<float> m_confs;
		vector<Rect> m_boxes;
		vector<int> m_classIds;
		vector<int> m_perfIndx;

		int m_inpWidth;
		int m_inpHeight;
		float m_confThro;
		float m_NMSThro;
		vector<string> m_classes;
	

};
