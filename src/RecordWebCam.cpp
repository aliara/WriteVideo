/*
 * Record video file from video device
 * File:   VidRecord.cpp
 * Author: Vern Raben
 * Copyright Raben Systems, Inc. 2012, All rights reserved.
 * @license: BSD 2 Clause License. See license included with this distribution in the file "license.txt"
 * Created on October 19, 2012, 2:39 PM
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>
#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Config.h"
//#include "license.txt"

using namespace std;
using namespace cv;

/**
  * String value for current time in GMT
  * @param curTime Pointer to time_t
  */
string currentTimeString(time_t *sec) {
    struct tm *ptm = gmtime (sec);
    ostringstream ostr;
    ostr<<setfill('0')<<setw(4)<< (ptm->tm_year + 1900)<< "-" << setw(2)<< (ptm->tm_mon+1)
            << "-" << setw(2)<<ptm->tm_mday << "T"<<setw(2)<<ptm->tm_hour
            << "h" <<setw(2) << ptm->tm_min << "m" << setw(2) << ptm->tm_sec;
    return ostr.str();
}

/**
 * Create a video file in specified directory. File name created from time specified.
 */
VideoWriter createVideoFile(String vidDir, int width, int height, int fps, int fourcc, time_t sec) {

    string timeStr = currentTimeString(&sec);
    string fileName = vidDir + timeStr + ".avi";
	cout << "Video file name = " << fileName << endl;
	Size frameSize = Size(width, height);
    VideoWriter vidWriter = VideoWriter(fileName, fourcc, fps,
        frameSize);
	return vidWriter;
}

/**
 * Write image to file. File name created from time specified
 */
void writeImageFile(string imgDir, Mat frame, string imgFmt, time_t sec) {
    string timeStr = currentTimeString(&sec);
    string fileName = imgDir + timeStr + "." + imgFmt;
    cout << "Image file = " << fileName << endl;
    imwrite(fileName, frame);
}

/*
 *
 */
int main(int argc, char** argv) {
    // Define control values and set defaults

    //string home = getenv("HOME");
	string home = "C:/Users/programador2/Desktop";
    Config config = Config();
    // Load config params from file (if it exists)

    string configFilename = home + "/" + ".vidrecordrc";
    config.loadFromFile(configFilename);

    string imgDir = home+ "/Pictures/";
    string imgFmt = "jpg";
    int imgInterval = 60; // seconds
    int imgNum = 0;
    bool isDisplayEnabled = true;
    string vidCodec = "DIVX";
    int vidDevice = 0;
    string vidDir = home + "/Videos/";
    int vidFps = 30;
    int vidInterval = 60; // seconds
    int vidNum = 0;
	time_t sec;


    // Add config params from command line arguments
    if (argc >= 1) {
        string name;
        string val;

        // Add command line arguments to Config class
        for (int n = 1; n < argc; n++) {
            string arg = argv[n];
            replace(arg.begin(),arg.end(),'-',' ');
            replace(arg.begin(),arg.end(),'=',' ');
            istringstream iss(arg);
            iss >> name;
            iss >> val;
            config.addVal(name, val);
        }

    }


    config.printVals();

    // Set control values from config map
    imgDir = config.getVal("imgdir", imgDir);
    imgFmt = config.getVal("imgfmt", imgFmt);
    imgInterval = config.getVal("imgint", imgInterval);
    imgNum = config.getVal("imgnum", imgNum);
    isDisplayEnabled = config.getVal("dis", isDisplayEnabled);
    vidCodec = config.getVal("vidcodec", vidCodec);
    vidDevice = config.getVal("viddev", vidDevice);
    vidDir = config.getVal("viddir", vidDir);
    vidFps = config.getVal("vidfps", 30);
    vidInterval = config.getVal("vidint", vidInterval);
    vidNum = config.getVal("vidnum", vidNum);

    // Determine vidCodec fourcc value from string
    int fourcc = CV_FOURCC(vidCodec[0],vidCodec[1],vidCodec[2], vidCodec[3]);

    // Open video device, set video frame width and height as driver often does not
    VideoCapture cap = VideoCapture(vidDevice);

    if (cap.isOpened()) {
        cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    } else {
    	cerr << "Error: No se encuentra la camara "<<vidDevice<<endl;
        return 1;
    }

    bool isEnabled = true;
    Mat frame;
    int key = 255;
    int delay = 1;
    bool isRecordingEnabled = vidNum > 0 ? true : false;
    bool isRecording = false;
    bool isImageCaptureEnabled = imgNum > 0 ? true : false;
    VideoWriter writer;
    time_t vidTime = 0;
    time_t vidDelta = 0;
    int vidTotal = 0;
    time_t imgTime = 0;
    time_t imgDelta = 0;
    int imgTotal = 0;
    long frameNum = 0;
    double fps = 0.0;

    // Frame capture loop
    while(isEnabled) {
        cap >> frame;
    	sec = time(NULL);
        frameNum++;

        if (isDisplayEnabled)
        {
        	if(!frame.empty())
        	imshow("Current Frame", frame);
            key = waitKey(delay) & 255;

            if (key != 255)
            {
                if ((key == 27) || (key == 'q'))
                {
                        isEnabled = false;
                }
                else if (key == 'r')
                {
                    isRecordingEnabled = ! isRecordingEnabled;
                    if ((isRecordingEnabled) && (vidNum == 0))
                    {
                            vidNum = 1;
                    }
                }
                else if (key == 'w') {
                    isImageCaptureEnabled = ! isImageCaptureEnabled;
                    if ((isImageCaptureEnabled) && (vidNum == 0))
                    {
                            imgNum = 1;
                    }
                }
                else
                {
                    cout << "key "<<key<<" not recognized" << endl;
                }
            }
        } else {
            isEnabled = isRecordingEnabled || isImageCaptureEnabled;
        }

        // Decide whether to create new video file
        if ((isRecordingEnabled) && (!isRecording)) {
            int width = (int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
            int height = (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT);
            writer = createVideoFile(vidDir, width, height, vidFps, fourcc, sec);
            vidTime = sec;
            isRecording = true;
            frameNum = 0;

        }

        // Write frame to video, calculate time interval and whether or not to create new video file
        if (isRecordingEnabled) {
            writer.write(frame);
            vidDelta = sec - vidTime;
            cout << "vidDelta "<<vidDelta<<" >= "<<vidInterval<<endl;

            if (vidDelta >= vidInterval) {
                isRecording = false;
                vidTotal = vidTotal + 1;
                cout << "Videos recorded =" << vidTotal << "/" << vidNum << endl;
                cout << "vidTotal="<<vidTotal<<" vidNum="<<vidNum<<endl;

                if (vidTotal >= vidNum) {
                    isRecordingEnabled = false;

                    if (vidDelta > 0) {
                            fps = frameNum / vidDelta;
                            frameNum = 0;
                    }

                    cout << "Recording completed fps=" << fps << endl;

                    if (isDisplayEnabled) {
                            writer = VideoWriter();
                    }

                }
            }

        }

        if (isImageCaptureEnabled) {
            imgDelta = (sec - imgTime);

            if (imgDelta >= imgInterval) {
                writeImageFile(imgDir, frame, imgFmt, sec);
                imgTime = sec;
                imgTotal = imgTotal + 1;

                if (imgTotal >= imgNum) {
                    isImageCaptureEnabled = false;
                }

            }
        }


    } // keep looping while enabled

    cap.release();
    exit(EXIT_SUCCESS);
}

