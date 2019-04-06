/*
    Created by Robert Jacobson on 2019-03-24.

*/

// #include <cmath>

#include <opencv2/imgproc.hpp>

#include "remap.h"

namespace projection{

float speedup_aux(float t){
    float expt = exp(t);
    return (expt - 1)/(expt+1)*t;
}

float speedup(float t){
    // s is how close to the end (t=1) is affected by the speedup.
    constexpr float s = 2.0f;
    return speedup_aux(speedup_aux(s*(t)))/(speedup_aux(speedup_aux(s)));
}

// Returns a number in (−π, π]. float OrthoToRectλ(float x, float y){ |x|, |y| <= 1. return
// atan2f(-y, x); }
float OrthoToRectλ(float x, float y){
    // |x|, |y| <= 1.
    return atan2(-y, x);
}

// Returns a number in [0, π]
float OrthoToRectφ(float x, float y){
    // |x|, |y| <= 1.
    float norm = hypotf(x, y);
    // if (norm > 1.0f) return 0.0;
    return acos(norm - trunc(norm));
}


float height;
float width;
float diameter; // The diameter of the output image.

// map_x
cv::Mat mapλ(static_cast<int>(diameter), static_cast<int>(diameter), CV_32FC1);
// map_y
cv::Mat mapφ(static_cast<int>(diameter), static_cast<int>(diameter), CV_32FC1);
cv::Mat mask(static_cast<int>(diameter), static_cast<int>(diameter), CV_8UC3);

/**
 * @brief Initializes the maps used in remap.
 * @param inwidth
 * @param inheight Typically half of inwidth
 * @param indiam Typically equal to inheight
 */
void initMaps(float inwidth= 3840.0f, float inheight= 1920.0f, float indiam = 0.0f){

    height   = inheight;
    width    = inwidth;
    diameter = indiam > 0 ? indiam : inheight;
    mapλ = cv::Mat(static_cast<int>(diameter), static_cast<int>(diameter), CV_32FC1);
    mapφ = cv::Mat(static_cast<int>(diameter), static_cast<int>(diameter), CV_32FC1);
    mask = cv::Mat(static_cast<int>(diameter), static_cast<int>(diameter), CV_8UC3);

    // map_x
    mapλ.forEach<float>([](float &x_coord, const int *pos) -> void {
        // Scale to [-1,1]X[-1,1]
        float x = pos[0]/diameter*2.0f - 1.0f;
        float y = pos[1]/diameter*2.0f - 1.0f;
        // Spread out region near sand.
        x_coord = (OrthoToRectλ(x, y)/M_PI + 1.0f)*0.5f*width;
    });
    // map_y
    mapφ.forEach<float>([](float &y_coord, const int *pos) -> void {
        // Scale to [-1,1]X[-1,1]
        float x = pos[0]/diameter*2.0f - 1.0f;
        float y = pos[1]/diameter*2.0f - 1.0f;
        // Spread out region above equator.
        y_coord = height*(1.0f - speedup((OrthoToRectφ(x*.8, y*.8)/M_PI + 0.5f)));
    });

    cv::circle(mask, cv::Point2d(diameter/2, diameter/2), diameter/2, cv::Scalar(255, 255, 255),
               cv::FILLED, cv::LineTypes::LINE_8);
}

void remap(cv::Mat &in, cv::Mat &out, int diam) {
    // Create the maps if they don't exist yet.
    if(mapφ.empty()) {
        initMaps(in.cols, in.rows, diam);
    }

    cv::remap(in, out, mapλ, mapφ, cv::INTER_LANCZOS4, cv::BORDER_WRAP, cv::Scalar(0, 0, 0));

    // Create black border.
    cv::bitwise_and(mask, out, out);
}

}

