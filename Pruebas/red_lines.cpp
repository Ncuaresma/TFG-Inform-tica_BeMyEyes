#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    ///cv::Mat input = cv::imread("C:/StackOverflow/Input/coloredLines.png");
    std::string image_path = cv::samples::findFile("Imagenes/Image0.jpeg");
    cv::Mat input  = cv::imread(image_path, cv::IMREAD_COLOR); // Load an image
    cv::imshow("input", input);
    // convert to HSV color space
    cv::Mat hsvImage;
    cv::cvtColor(input, hsvImage, CV_BGR2HSV);
    cv::imshow("hsv", hsvImage);
    // split the channels
    std::vector<cv::Mat> hsvChannels;
    cv::split(hsvImage, hsvChannels);

    // hue channels tells you the color tone, if saturation and value aren't too low.

    // red color is a special case, because the hue space is circular and red is exactly at the beginning/end of the circle.
    // in literature, hue space goes from 0 to 360 degrees, but OpenCV rescales the range to 0 up to 180, because 360 does not fit in a single byte. Alternatively there is another mode where 0..360 is rescaled to 0..255 but this isn't as common.
    int hueValue = 0; // red color
    int hueRange = 15; // how much difference from the desired color we want to include to the result If you increase this value, for example a red color would detect some orange values, too.

    int minSaturation = 50; // I'm not sure which value is good here...
    int minValue = 50; // not sure whether 50 is a good min value here...

    cv::Mat hueImage = hsvChannels[0]; // [hue, saturation, value]

    // is the color within the lower hue range?
    cv::Mat hueMask;
    cv::inRange(hueImage, hueValue - hueRange, hueValue + hueRange, hueMask);

    // if the desired color is near the border of the hue space, check the other side too:
    // TODO: this won't work if "hueValue + hueRange > 180" - maybe use two different if-cases instead... with int lowerHueValue = hueValue - 180
    if (hueValue - hueRange < 0 || hueValue + hueRange > 180)
    {
        cv::Mat hueMaskUpper;
        int upperHueValue = hueValue + 180; // in reality this would be + 360 instead
        cv::inRange(hueImage, upperHueValue - hueRange, upperHueValue + hueRange, hueMaskUpper);

        // add this mask to the other one
        hueMask = hueMask | hueMaskUpper;
    }

    // now we have to filter out all the pixels where saturation and value do not fit the limits:
    cv::Mat saturationMask = hsvChannels[1] > minSaturation;
    cv::Mat valueMask = hsvChannels[2] > minValue;

    hueMask = (hueMask & saturationMask) & valueMask;

    cv::imshow("desired color", hueMask);

    // now perform the line detection
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(hueMask, lines, 1, CV_PI / 360, 50, 50, 10);

    // draw the result as big green lines:
    for (unsigned int i = 0; i < lines.size(); ++i)
    {
        cv::line(input, cv::Point(lines[i][0], lines[i][1]), cv::Point(lines[i][2], lines[i][3]), cv::Scalar(0, 255, 0), 5);
    }


    cv::imwrite("C:/StackOverflow/Output/coloredLines_mask.png", hueMask);
    cv::imwrite("C:/StackOverflow/Output/coloredLines_detection.png", input);

    cv::imshow("input", input);
    cv::waitKey(0);
    return 0;
}