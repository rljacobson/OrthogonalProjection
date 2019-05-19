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

int batchProcess(const std::string &file_in, const std::string &file_out, int min, int max) {
    std::cout << "Processing images...\n";

    for (long int j = min; j <= max; j++) {
        std::string inpath  = fmt::format(file_in, j);
        std::string outpath = fmt::format(file_out, j);
        std::cout << inpath << '\n';

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

    std::cout << "Done processing " << (max-min+1) << std::endl;

    return EXIT_SUCCESS;
}

void printUsage() {
    std::cout << "Usage:\n\n\tproject inpattern outpattern [max | min max]\n\n"
                 "where\n\n\tinpatter is input/frames/path/in{:04d}.png\n"
                 "\toutpatter is output/frames/path/out{:04d}.png\n"
                 "\tmin (if given) and max are the 1-indexed number of the frames to process." <<
                 std::endl;
}

int main(int argc, char **argv) {
    // std::string file_in(
    //         "/Users/rljacobson/Downloads/out/SDR-FAB/out{:04d}.png");
    //
    // std::string file_out("/Users/rljacobson/Downloads/out/projectedSDR-FAB/out{:04d}.png");

    std::cout << "Value of argc is " << argc << ".\n";

    int min = -1;
    int max = -1;

    if(argc == 5){
        std::string min_str(argv[3]);
        std::string max_str(argv[4]);
        min = std::stoi(min_str);
        max = std::stoi(max_str);
    } else if(argc==4) {
        min = 1;
        std::string max_str(argv[3]);
        max = std::stoi(max_str);
    } else {
        std::cout << "Frames: " <<  min << " to " << max << '\n';
        printUsage();
        return EXIT_FAILURE;
    }

    std::cout << "Frames: " <<  min << " to " << max << '\n';


    if(max <= 0 || min <= 0 || min > max){
        printUsage();
        return EXIT_FAILURE;
    }

    std::string file_in(argv[1]);
    std::string file_out(argv[2]);

    return batchProcess(file_in, file_out, min, max);
}
