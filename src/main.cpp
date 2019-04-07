/*
    Created by Robert Jacobson on 2019-03-18.

*/

#include <iostream>
#include <fmt/format.h>
#include <vector>

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

#include "colorbalance.h"
#include "remap.h"

int batchProcess(const std::string &file_in, const std::string &file_out, long int max) {
    std::cout << "Processing images...\n";

    for (int j = 1; j < max; j++) {
        std::string inpath  = fmt::format(file_in, j);
        std::string outpath = fmt::format(file_out, j);

        cv::Mat image = cv::imread(inpath, 1);
        if (image.empty()) {
            std::cout << "No image data for filename:" << inpath << std::endl;
            return -1;
        }

        ColorBalance(image, image);
        // ColorToneMapping compresses the luminance to make an HDR effect.
        // We choose instead to keep all luminance data.

        cv::Mat result(1920, 1920, CV_8UC3); // NOLINT(hicpp-signed-bitwise)
        projection::remap(image, result);

        cv::imwrite(outpath, result);

    } // End for

    std::cout << "Done!" << std::endl;

    return EXIT_SUCCESS;
}

void printUsage() {
    std::cout << "Usage:\n\n\tproject inpattern outpattern n\n\n"
                 "where\n\n\tinpatter is input/frames/path/in{:04d}.png\n"
                 "\tinpatter is output/frames/path/out{:04d}.png\n"
                 "\tn is the 1-indexed number of frames total." << std::endl;
}

int main(int argc, char **argv) {
    // std::string file_in(
    //         "/Users/rljacobson/Downloads/out/SDR-FAB/out{:04d}.png");
    //
    // std::string file_out("/Users/rljacobson/Downloads/out/projectedSDR-FAB/out{:04d}.png");


    if(argc < 3){
        printUsage();
        return EXIT_FAILURE;
    }

    long int max = strtol(argv[3], nullptr, 10);

    if(max <= 0){
        printUsage();
        return EXIT_FAILURE;
    }

    std::string file_in(argv[1]);
    std::string file_out(argv[2]);

    return batchProcess(file_in, file_out, max);
}
