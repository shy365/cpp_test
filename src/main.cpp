#include <iostream>
#include <queue>
#include <vector>
#include <unistd.h>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <stdlib.h>
#include <dirent.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

#include "utils.h"
#include "object_detect.h"


std::chrono::high_resolution_clock::time_point detect_start;
std::chrono::high_resolution_clock::time_point detect_end;

using namespace std;
using namespace cv;

std::mutex read_mtx,detect_mtx,afterure_mtx,display_mtx;
std::queue<cv::Mat> read_queue,detect_queue,afterure_queue,display_queue,output_queue;

static int frameWidth;
static int frameHeight;

Detection detection;

Result read_frame(int cpuId)
{
   static string video_path = "/home/fuhao/project/yolov4/data/car.mp4";
	//绑核运行
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(cpuId,&mask);
	if(pthread_setaffinity_np(pthread_self(),sizeof(mask),&mask) < 0)
		ERROR_LOG("set thread affinity failed");
	
	INFO_LOG("Bind %s process to CPU %d",__func__,cpuId);
	cv::VideoCapture cap(video_path);
	if(!cap.isOpened())
	{
		ERROR_LOG("open video failed");
		return FAILED;

	}
	else
	{
		INFO_LOG("open video success");
		INFO_LOG("总帧数:%d",(int)cap.get(cv::CAP_PROP_FRAME_COUNT));
		frameWidth = (int)cap.get(cv::CAP_PROP_FRAME_WIDTH);
		frameHeight = (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT);
		INFO_LOG("frameWidth: %d",frameWidth);
		INFO_LOG("frameHeight: %d",frameHeight)
	}
	

	while(true)
	{
		cv::Mat frame;
		if(!cap.read(frame))
			break;
		read_mtx.lock();
		read_queue.push(frame);
		read_mtx.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(40));

	}

	cap.release();
	
	INFO_LOG("read video success");
	return SUCCESS;
}


Result object_detection(int cpuId)
{

	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(cpuId,&mask);

	if(pthread_setaffinity_np(pthread_self(),sizeof(mask),&mask) < 0)
	{
		ERROR_LOG("set cpu2 affinity failed");
		return FAILED;
	}
	INFO_LOG("Bind %s process to CPU %d ",__func__,cpuId);

	detection = Detection();
	detection.Initialize(frameWidth,frameHeight);

	while(true)
	{
		read_mtx.lock();
		if(!read_queue.empty()){
			
			cv::Mat src1 = read_queue.front();
			read_queue.pop();
			read_mtx.unlock();
			detect_start = std::chrono::high_resolution_clock::now();//起始时间
			detection.Detecting(src1);
			detect_end = std::chrono::high_resolution_clock::now();//结束时间
			
			detect_mtx.lock();
			detect_queue.push(detection.GetFrame());
			detect_mtx.unlock();

			std::chrono::duration <double, std::milli > time_span = detect_end - detect_start;
			INFO_LOG("detect speedtime is %.2lf ms",time_span);
		}
		else
		{
			read_mtx.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

	}

}


Result afterure_frame(int cpuId)
{
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(cpuId,&mask);
	if(pthread_setaffinity_np(pthread_self(),sizeof(mask),&mask) < 0)
	{
		ERROR_LOG("set CPU3 affinity failed");
		return FAILED;
	}

	INFO_LOG("Bind process %s to CPU %d",__func__,cpuId);

	while(true)
	{
		detect_mtx.lock();
		if(!detect_queue.empty())
		{
			cv::Mat src = detect_queue.front();
			detect_queue.pop();
			detect_mtx.unlock();
			detection.Afterure(src);
			imshow("object_detect",src);
			if(waitKey(25) == 27)
			{
				break;
			}
			detection.Dump();
		}
		else
		{
			detect_mtx.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;

		}
	}
	
	destroyAllWindows();
	return SUCCESS;

}


int main(int argc, char* argv[])
{
	thread t1(read_frame,1);
	thread t2(object_detection,2);
	thread t3(afterure_frame,3);
//	thread t4(display_frame);
//	thread t5(output_frame);


	t1.join();
	t2.join();
	t3.join();
//	t4,join();
//	t5.join();




}



