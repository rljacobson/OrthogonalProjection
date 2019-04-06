//
// Created by Robert Jacobson on 2019-03-20.
//

#pragma once

#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

/**
 * @brief Perform "the Simplest Color Balancing algorithm." Users should use `ColorBalance` instead.
 *
 * Performs the color balance (a.k.a. white balance) algorithm given in the following:
 *
 *     Nicolas Limare, Jose-Luis Lisani, Jean-Michel Morel, Ana Belén Petro, and Catalina Sbert,
 *     Simplest Color Balance, Image Processing On Line, 1 (2011), pp. 297–315.
 *     https://doi.org/10.5201/ipol.2011.llmps-scb
 *
 * Users should use ::ColorBalance instead.
 *
 * @param in The input matrix.
 * @param out The output matrix.
 * @param percent A percentage cutoff, `percent/2` from both bottom and top, which the algorithm
 * will ignore when it stretches the values.
 */
void SimplestCB(cv::Mat& in, cv::Mat& out, float percent) {
    assert(in.channels() == 3);
    assert(percent > 0 && percent < 100);
    float half_percent = percent / 200.0f;

    std::vector<cv::Mat> tmpsplit; split(in, tmpsplit);
    for(int i=0;i<3;i++) {
        //find the low and high precentile values (based on the input percentile)
        cv::Mat flat; tmpsplit[i].reshape(1,1).copyTo(flat);
        cv::sort(flat,flat,cv::SORT_EVERY_ROW + cv::SORT_ASCENDING);
        int lowval = flat.at<uchar>(cvFloor(((float)flat.cols) * half_percent));
        int highval = flat.at<uchar>(cvCeil(((float)flat.cols) * (1.0 - half_percent)));
        // std::cout << lowval << " " << highval << std::endl;

        //saturate below the low percentile and above the high percentile
        tmpsplit[i].setTo(lowval,tmpsplit[i] < lowval);
        tmpsplit[i].setTo(highval,tmpsplit[i] > highval);

        //scale the channel
        normalize(tmpsplit[i],tmpsplit[i],0,255, cv::NORM_MINMAX);
    }
    merge(tmpsplit,out);
}

// The following matrices are ubiquitous in the literature.
static const cv::Mat_<float> mRGB2LMS = (cv::Mat_<float>(3, 3) <<
        17.8824, 43.5161, 4.1194,
        3.4557, 27.1554, 3.8671,
        0.0300, 0.1843, 1.4671);

static const cv::Mat_<float> mLMS2RGB = (cv::Mat_<float>(3, 3) <<
        0.0809, -0.1305, 0.1167,
        -0.0102, 0.0540, -0.1136,
        -0.0004, -0.0041, 0.6935);

// The following assume XYZ input.
// Hunt-Pointer-Estevez normalized to D65
static const cv::Mat_<float> mVonKries = (cv::Mat_<float>(3, 3) <<
        .40024, .7076, -.08081,
        -.2263, 1.16532, 0.0457,
        0.0, 0.0, .91822);
static const cv::Mat_<float> mBradford = (cv::Mat_<float>(3, 3) <<
        .8951, .2664, -.1614,
        -.7502, 1.7135, 0.0367,
        0.0389, -0.0685, 1.0296);
static const cv::Mat_<float> mBradfordInv = (cv::Mat_<float>(3, 3) <<
        0.986993,-0.147054,0.159963,
        0.432305,0.51836,0.0492912,
        -0.00852866,0.0400428,0.968487);

static const cv::Mat_<float> mSharp = (cv::Mat_<float>(3, 3) <<
        1.2694, -.0988, -.1706,
        -.8364, 1.8006, 0.0357,
        0.0297, -0.0315, 1.0018);
static const cv::Mat_<float> mCMCCAT2000 = (cv::Mat_<float>(3, 3) <<
        .7982, .3389, -.1371,
        -.5918, 1.5512, 0.0406,
        0.0008, 0.239, 0.9753);
static const cv::Mat_<float> mCAT02 = (cv::Mat_<float>(3, 3) <<
        0.7328, 0.4296, -.1624,
        -.7036, 1.6975, 0.0061,
        0.0030, 0.0136, 0.9834);

// D65 = 95.047, 100.00, 108.883
static const cv::Mat_<float> vD65 = (cv::Mat_<float>(3, 1) << 0.95047,	1.00000, 1.08883);
cv::Mat_<float> mD65 = cv::Mat::diag(vD65);

// Inverse Transformation from LMS Color Space -> BGR Color Space
// void lms2bgr(const cv::Mat &msrc, cv::Mat mdst){
//     cv::transform(msrc, mdst, mLMS2RGB);
//     cv::cvtColor(mdst, mdst, cv::COLOR_RGB2BGR );
// }

/*!
 * @brief Perform Bradford color balance (a.k.a. white balance).
 * @param msrc
 * @param mdst
 * @param percent
 */
void ColorBalance(const cv::Mat  &msrc, cv::Mat &mdst, float percent = 10.0f){
    cv::cvtColor(msrc, mdst, cv::COLOR_BGR2XYZ );
    SimplestCB(mdst, mdst, percent);
    cv::transform(mdst, mdst, mBradford);
    cv::cvtColor(mdst, mdst, cv::COLOR_RGB2BGR);
}

