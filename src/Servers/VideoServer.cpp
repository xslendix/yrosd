#include <Servers/VideoServer.h>

#include <Common.h>
#include <Config.h>

#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>
#include <nadjieb/mjpeg_streamer.hpp>

using nadjieb::MJPEGStreamer;

namespace Server {

void Video::run()
{
    Server::Run();

    if (!Configuration::the().video_streaming()->enabled)
        return;

    cv::VideoCapture camera(Configuration::the().video_streaming()->camera);
    if (!camera.isOpened()) {
        spdlog::error("VideoServer: Failed opening camera index {}. Video streaming disabled.",
            Configuration::the().video_streaming()->camera);
        Configuration::the().video_streaming()->enabled = false;
        m_running = false;

        return;
    }

    MJPEGStreamer streamer;
    streamer.start(Configuration::the().video_streaming()->port);

    std::vector<int> params = {
        cv::IMWRITE_JPEG_QUALITY,
        Configuration::the().video_streaming()->jpeg_quality
    };

    spdlog::info("VideoServer: Awaiting connections on port {} using camera index {}.",
        Configuration::the().video_streaming()->port,
        Configuration::the().video_streaming()->camera);

    while (streamer.isAlive()) {
        if (m_running == false) {
            spdlog::info("VideoServer: Stopping...");
            streamer.stop();
            break;
        }

        cv::Mat frame;
        camera >> frame;
        if (frame.empty())
            continue;

        std::vector<uchar> buffer_bgr;
        cv::imencode(".jpg", frame, buffer_bgr, params);

        streamer.publish("/video.mjpg", std::string(buffer_bgr.begin(), buffer_bgr.end()));
    }

    camera.release();
    streamer.stop();
}

}
