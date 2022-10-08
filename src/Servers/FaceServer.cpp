#include <Servers/FaceServer.h>

#include <Servers/MainServer.h>

#include <Common.h>
#include <Config.h>

#include <opencv2/core.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>

namespace Server {

float servo_speed_x, servo_speed_y;

void
in_center(int& x, int& y, int& width, int& height, bool& ret1, bool& ret2)
{
  // FIXME: Add configuration file entry for this
  int rect_xy_size = cv::min(width, height) / 6;

  ret1 = x > (width / 2 - rect_xy_size) && x < (width / 2 + rect_xy_size);
  ret2 = y < (height / 2 + rect_xy_size) && y > (height / 2 - rect_xy_size);
}

short
calculate_direction(int& distance_pixels, int& limit)
{
  // Calculate servo speed
  return distance_pixels > limit / 2 ? 1 : -1;
}

void
variable_swap(int& a, int& b)
{
  int temp = a;
  a = b;
  b = temp;
}

void
Face::Run()
{
  std::vector<cv::Rect> faces;

  Server::Run();

  if (Configuration::the().face_tracking()->enabled == false)
    return;

  spdlog::info("FaceServer: Initialising...");

  while (servo_controller == nullptr) {
    sleep(1);
  }

  cv::VideoCapture camera(Configuration::the().face_tracking()->head_camera);
  if (!camera.isOpened()) {
    spdlog::error("FaceServer: Failed opening camera index {}. Video streaming disabled.",
        Configuration::the().face_tracking()->head_camera);
    Configuration::the().face_tracking()->enabled = false;
    m_running = false;

    return;
  }

  spdlog::info("FaceServer: Initialized!");

  auto classifier = cv::CascadeClassifier("haarcascade_frontalface_default.xml");
  for (;;) {
    int rotation,
        width, height;
    int center_x = 0, center_y = 0;
    cv::Mat frame, gray;

    if (m_running == false) {
      spdlog::info("FaceServer: Stopping...");
      break;
    }

    camera >> frame;

    if (frame.empty())
      continue;

    width = frame.cols;
    height = frame.rows;

    // Rotate frame, if enabled
    switch (Configuration::the().face_tracking()->rotate_camera) {
    case 90:
      rotation = cv::ROTATE_90_CLOCKWISE;
      variable_swap(width, height);
      break;
    case 180:
      rotation = cv::ROTATE_180;
      break;
    case 270:
      rotation = cv::ROTATE_90_COUNTERCLOCKWISE;
      variable_swap(width, height);
      break;
    default:
      rotation = 0;
    }

    if (rotation != 0)
      cv::rotate(frame, frame, rotation);

    // Convert to grayscale
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // Detect faces in frame
    classifier.detectMultiScale(gray, faces, 1.3, 5);

    // Find center point with all faces
    for (auto face : faces) {
      center_x += face.x + face.width / 2;
      center_y += face.y + face.height / 2;
    }

    if (faces.size() > 0) {
      bool x, y;

      center_x /= faces.size();
      center_y /= faces.size();

      in_center(center_x, center_y, width, height, x, y);

      servo_speed_x = x ? 0 : -calculate_direction(center_x, width) * Configuration::the().face_tracking()->speed_x;
      servo_speed_y = y ? 0 : -calculate_direction(center_y, height) * Configuration::the().face_tracking()->speed_y;
    } else {
      servo_speed_x = servo_speed_y = 0;
    }

    int rect_xy_size = cv::min(width, height) / 6;

    servo_speed_x /= 180.;
    servo_speed_y /= 180.;
    servo_speed_x *= 4096.;
    servo_speed_y *= 4096.;

    servo_controller->setPin(Configuration::the().head()->yaw, servo_speed_x);
    servo_controller->setPin(Configuration::the().head()->pitch, servo_speed_y);
  }

  camera.release();
}

}
