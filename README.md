# liteCV
**_liteCV_** is lightweight image processing library for C++11.  
Unlike _OpenCV_, **_liteCV_** must be **PORTABLE**.  
Unlike _OpenCV_, **_liteCV_** must be **INDEPENDENCE**.
Unlike _OpenCV_, **_liteCV_** must be **EASY-TO-USE**.

# Achievements
The main achievement of *_liteCV_* is providing equivalence with OpenCV.  
Especially, we are focusing implementing OpenCV's `core`, `imgcodec`, `imgproc` and `highgui`.

## Data Structure (`core`)
1. Element (equivalent to `cv::Vec`/`cv::Scalar`)
2. Size (equivalent to `cv::Size`)
3. Point (equivalent to `cv::Point`)
4. Rect (equivalent to `cv::Rect`)
5. Matrix (equivalent to `cv::Mat`)

## Image I/O (`imgcodec`)
1. Image Reading/Writing (equivalent to `cv::imread`/`cv::imwrite`)
2. Image Encoding/Decoding (equivalent to `cv::imencode`/`cv::imdecode`)

## Image Processing (`imgproc`)
1. Color Conversion (equivalent to `cv::cvtColor`)
2. Image Filters (equivalent to `cv::filter2D`, etc...)

## High-level GUI
1. Display Image (equivalent to `cv::imshow`/`cv::namedWindow`)
2. Keyboad Input (equivalent to `cv::waitKey`)
