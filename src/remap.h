/*
    Created by Robert Jacobson on 2019-03-24.

*/

#pragma once

#include <opencv2/imgproc.hpp>

/**
 * The projection namespace provides facilities to project from an equirectangular projection to an
 * orthographic projection by giving the backwards transformation $T\colon D \to S^2_{\text{upper}}$
 * from the unit disk to the upper half-sphere given by $(x, y) \to (λ, φ)$. In fact, $T$ extends to
 * $[-1, 1]X[-1,1]$ (or any larger region) by "wrapping" around the sphere.
 *
 * The remap function, named after its more general cousin in OpenCV, performs the actual
 * projection:
 *
 * ```cpp
 * void projection::remap(cv::Mat &in, cv::Mat &out);
 * ```
 *
 */
namespace projection{

    /**
     * @brief Orthogonal projection from the upper half-sphere to the disk.
     *
     * Gives the orthogonal projection of the upper half-sphere represented as an equirectangual
     * projected image `in` to a disk. If `diam` is not provided, it is set to `in.rows` (height).
     *
     * This is _not_ actually the orthogonal projection; it spreads out the annulus right above
     * the equator to give more detail in that region at the expense of detail close to and at
     * the equator.
     *
     * @param in
     * @param out
     * @param diam
     */
    void remap(cv::Mat &in, cv::Mat &out, int diam = 0);

    /**
     * @brief Converts (x, y) -> λ (Longitude)
     *
     * Given a point (x, y) in [-1, 1]X[-1, 1], returns a number in (−π, π] representing
     * the orthographic projection of the point to the λ-coordinate in the upper-half sphere.
     * @param x x-coordinate
     * @param y y-coordinate
     * @return λ-coordinate.
     */
    float OrthoToRectλ(float x, float y);

    /**
     * @brief @brief Converts (x, y) -> φ (Latitude)
     *
     * Given a point (x, y) in [-1, 1]X[-1, 1], returns a number in [0, π] representing
     * the orthographic projection of the point to the φ-coordinate in the upper-half sphere.
     * @param x x-coordinate
     * @param y y-coordinate
     * @return φ-coordinate.
     */
    float OrthoToRectφ(float x, float y);

}
