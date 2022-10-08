#include <Common.h>
#include <Config.h>

#include <filesystem>
#include <arpa/inet.h>

#include <toml++/toml.h>

namespace Config {

static unique_ptr<Configuration> s_the = nullptr;

string get_configuration_file_path();
toml::table get_configuration_file_data();

DriveMode string_to_drive_mode(string drive_mode);

bool
validate_ip_address(const string& ipAddress)
{
  struct sockaddr_in sa;
  int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
  return result != 1;
}

Configuration&
Configuration::the()
{
  if (!s_the) {
    unique_ptr<Configuration> new_the(std::make_unique<Configuration>(get_configuration_file_data()));
    s_the = std::move(new_the);
  }
  return *s_the;
}

string
Configuration::as_printable_string()
{
  // TODO: Use macros instead
  stringstream ss;

  string back_left_string;
  for (int i : m_motors_config->back_left)
    back_left_string += std::to_string(i) + " ";
  string back_right_string;
  for (int i : m_motors_config->back_right)
    back_right_string += std::to_string(i) + " ";
  string front_left_string;
  for (int i : m_motors_config->front_left)
    front_left_string += std::to_string(i) + " ";
  string front_right_string;
  for (int i : m_motors_config->front_right)
    front_right_string += std::to_string(i) + " ";

  ss << "Server:"
     << "\n  Port: " << m_server_config->port
     << "\n  Host: " << m_server_config->host;
  ss << "\nMotors:"
     << "\n  Enabled: " << m_motors_config->enabled
     << "\n  BackLeft: " << back_left_string
     << "\n  BackRight: " << back_right_string
     << "\n  FrontLeft: " << front_left_string
     << "\n  FrontRight: " << front_right_string
     << "\n  LeftInverted: " << m_motors_config->left_inverted
     << "\n  RightInverted: " << m_motors_config->right_inverted;
  // TODO: Add DriveMode
  ss << "\nHead:"
     << "\n  Enabled: " << m_head_config->enabled
     << "\n  Yaw: " << m_head_config->yaw
     << "\n  Pitch: " << m_head_config->pitch
     << "\n  InitYaw: " << m_head_config->init_yaw
     << "\n  InitPitch: " << m_head_config->init_pitch;
  ss << "\nHand:"
     << "\n  Enabled: " << m_head_config->enabled
     << "\n  Left: " << m_hand_config->left
     << "\n  Right: " << m_hand_config->right
     << "\n  OpenAngle: " << m_hand_config->open_angle
     << "\n  CloseAngle: " << m_hand_config->close_angle;
  ss << "\nWrists:"
     << "\n  Enabled: " << m_wrists_config->enabled
     << "\n  Left: " << m_wrists_config->left
     << "\n  Right: " << m_wrists_config->right
     << "\n  MaxAngle: " << m_wrists_config->max_angle
     << "\n  MinAngle: " << m_wrists_config->min_angle;
  ss << "\nFaceTracking:"
     << "\n  Enabled: " << m_face_tracking_config->enabled
     << "\n  HeadCamera: " << m_face_tracking_config->head_camera
     << "\n  RotateCamera: " << m_face_tracking_config->rotate_camera
     << "\n  SpeedX: " << m_face_tracking_config->speed_x
     << "\n  SpeedY: " << m_face_tracking_config->speed_y;
  ss << "\nFace:"
     << "\n  Emotions: " << m_face_config->emotions;
  ss << "\nVideoStreaming:"
     << "\n  Enabled: " << m_video_streaming_config->enabled
     << "\n  Port: " << m_video_streaming_config->port
     << "\n  Camera: " << m_video_streaming_config->camera
     << "\n  Width: " << m_video_streaming_config->width
     << "\n  Height: " << m_video_streaming_config->height;

  return ss.str();
}

Configuration::Configuration(toml::table toml_data)
{
  reload(toml_data);
}

void
Configuration::reload(toml::table toml_data)
{
  if (toml_data.empty())
    return;

#pragma region Server

  m_server_config->host = toml_data["Server"]["Host"].value_or(m_server_config->host);

  m_server_config->port = toml_data["Server"]["Port"].value_or(m_server_config->port);

#pragma endregion

#pragma region Motors

  m_motors_config->enabled = toml_data["Motors"]["Enabled"].value_or(m_motors_config->enabled);

  auto back_left = toml_data["Motors"]["BackLeft"].as_array();
  m_motors_config->back_left[0] = back_left->get(0)->value_or(m_motors_config->back_left[0]);
  m_motors_config->back_left[1] = back_left->get(1)->value_or(m_motors_config->back_left[1]);
  m_motors_config->back_left[2] = back_left->get(2)->value_or(m_motors_config->back_left[2]);
  m_motors_config->back_left[3] = back_left->get(3)->value_or(m_motors_config->back_left[3]);

  auto back_right = toml_data["Motors"]["BackRight"].as_array();
  m_motors_config->back_right[0] = back_right->get(0)->value_or(m_motors_config->back_right[0]);
  m_motors_config->back_right[1] = back_right->get(1)->value_or(m_motors_config->back_right[1]);
  m_motors_config->back_right[2] = back_right->get(2)->value_or(m_motors_config->back_right[2]);
  m_motors_config->back_right[3] = back_right->get(3)->value_or(m_motors_config->back_right[3]);

  auto front_left = toml_data["Motors"]["FrontLeft"].as_array();
  m_motors_config->front_left[0] = front_left->get(0)->value_or(m_motors_config->front_left[0]);
  m_motors_config->front_left[1] = front_left->get(1)->value_or(m_motors_config->front_left[1]);
  m_motors_config->front_left[2] = front_left->get(2)->value_or(m_motors_config->front_left[2]);
  m_motors_config->front_left[3] = front_left->get(3)->value_or(m_motors_config->front_left[3]);

  auto front_right = toml_data["Motors"]["FrontRight"].as_array();
  m_motors_config->front_right[0] = front_right->get(0)->value_or(m_motors_config->front_right[0]);
  m_motors_config->front_right[1] = front_right->get(1)->value_or(m_motors_config->front_right[1]);
  m_motors_config->front_right[2] = front_right->get(2)->value_or(m_motors_config->front_right[2]);
  m_motors_config->front_right[3] = front_right->get(3)->value_or(m_motors_config->front_right[3]);

  m_motors_config->left_inverted = toml_data["Motors"]["LeftInverted"].value_or(m_motors_config->left_inverted);
  m_motors_config->right_inverted = toml_data["Motors"]["RightInverted"].value_or(m_motors_config->right_inverted);

  m_motors_config->drive_mode = string_to_drive_mode(toml_data["Motors"]["DriveMode"].value_or("Tank"));

#pragma endregion

#pragma region Head

  m_head_config->enabled = toml_data["Head"]["Enabled"].value_or(m_head_config->enabled);

  m_head_config->yaw = toml_data["Head"]["Yaw"].value_or(m_head_config->yaw);
  m_head_config->pitch = toml_data["Head"]["Pitch"].value_or(m_head_config->pitch);

  m_head_config->init_yaw = toml_data["Head"]["InitYaw"].value_or(m_head_config->init_yaw);
  m_head_config->init_pitch = toml_data["Head"]["InitPitch"].value_or(m_head_config->init_pitch);

#pragma endregion

#pragma region Hand

  m_hand_config->enabled = toml_data["Hand"]["Enabled"].value_or(m_hand_config->enabled);

  m_hand_config->left = toml_data["Hand"]["Left"].value_or(m_hand_config->left);
  m_hand_config->right = toml_data["Hand"]["Right"].value_or(m_hand_config->right);

  m_hand_config->open_angle = toml_data["Hand"]["OpenAngle"].value_or(m_hand_config->open_angle);
  m_hand_config->close_angle = toml_data["Hand"]["CloseAngle"].value_or(m_hand_config->close_angle);

#pragma endregion

#pragma region Wrists

  m_wrists_config->enabled = toml_data["Wrists"]["Enabled"].value_or(m_wrists_config->enabled);

  m_wrists_config->left = toml_data["Wrists"]["Left"].value_or(m_wrists_config->left);
  m_wrists_config->right = toml_data["Wrists"]["Right"].value_or(m_wrists_config->right);

  m_wrists_config->max_angle = toml_data["Wrists"]["MaxAngle"].value_or(m_wrists_config->max_angle);
  m_wrists_config->min_angle = toml_data["Wrists"]["MinAngle"].value_or(m_wrists_config->min_angle);

#pragma endregion

#pragma region Face Tracking

  m_face_tracking_config->enabled = toml_data["FaceTracking"]["Enabled"].value_or(m_face_tracking_config->enabled);

  m_face_tracking_config->head_camera = toml_data["FaceTracking"]["HeadCamera"].value_or(m_face_tracking_config->head_camera);
  m_face_tracking_config->rotate_camera = toml_data["FaceTracking"]["RotateCamera"].value_or(m_face_tracking_config->rotate_camera);

  m_face_tracking_config->speed_x = toml_data["FaceTracking"]["SpeedX"].value_or(m_face_tracking_config->speed_x);
  m_face_tracking_config->speed_y = toml_data["FaceTracking"]["SpeedY"].value_or(m_face_tracking_config->speed_y);

#pragma endregion

#pragma region Face

  m_face_config->emotions = toml_data["Face"]["Emotions"].value_or(m_face_config->emotions);

#pragma endregion

#pragma region Video Streaming

  m_video_streaming_config->enabled = toml_data["VideoStreaming"]["Enabled"].value_or(m_video_streaming_config->enabled);

  m_video_streaming_config->port = toml_data["VideoStreaming"]["Port"].value_or(m_video_streaming_config->port);
  m_video_streaming_config->camera = toml_data["VideoStreaming"]["Camera"].value_or(m_video_streaming_config->camera);

  m_video_streaming_config->width = toml_data["VideoStreaming"]["Width"].value_or(m_video_streaming_config->width);
  m_video_streaming_config->height = toml_data["VideoStreaming"]["Height"].value_or(m_video_streaming_config->height);

  m_video_streaming_config->framerate = toml_data["VideoStreaming"]["FrameRate"].value_or(m_video_streaming_config->framerate);
  m_video_streaming_config->jpeg_quality = toml_data["VideoStreaming"]["JPEGQuality"].value_or(m_video_streaming_config->jpeg_quality);

#pragma endregion

#pragma region Configuration checking

  spdlog::debug("Checking configuration");

  if (validate_ip_address(m_server_config->host)) {
    Server temp_server_config;
    spdlog::warn("Config: Invalid IP address in the configuration, defaulting to {}.", temp_server_config.host);
    m_server_config->host = temp_server_config.host;
  }

  if (m_server_config->port < 0 || m_server_config->port > 65535) {
    spdlog::warn("Config: Invalid port range for main server. Exiting...");
    std::exit(1);
  }

  if (m_server_config->port < 0 || m_server_config->port > 65535) {
    spdlog::warn("Config: Invalid port range for motors. Turning off motor control...");
    m_motors_config->enabled = false;
  }

  if (std::getenv("DISPLAY") != NULL)
    m_gui_present = true;

  if (m_face_config->emotions) {
    if (m_gui_present == false) {
      spdlog::warn("Config: No X11 server running, disabling face emotions.");
      m_face_config->emotions = false;
    }
  }

  if (m_face_tracking_config->enabled) {
    if (!std::filesystem::exists("/dev/video" + std::to_string(m_face_tracking_config->head_camera))) {
      spdlog::warn("Config: Camera for Face Tracking of id {} not found! Turning off Face Tracking.", m_face_tracking_config->head_camera);
      m_face_tracking_config->enabled = false;
    }
  }

  if (m_video_streaming_config->enabled) {
    if (m_video_streaming_config->port < 0 || m_video_streaming_config->port > 65535) {
      spdlog::warn("Config: Invalid port range for Video Streaming. Turning off video streaming.");
      m_video_streaming_config->enabled = false;
    }
  }

  if (m_video_streaming_config->enabled) {
    if (m_video_streaming_config->port == m_server_config->port) {
      spdlog::warn("Config: Port for Video Streaming is the same as the one for the main server! Turning off video streaming.");
      m_video_streaming_config->enabled = false;
    }
  }

  if (m_video_streaming_config->enabled) {
    if (!std::filesystem::exists("/dev/video" + std::to_string(m_video_streaming_config->camera))) {
      spdlog::warn("Config: Camera for Video Streaming of id {} not found! Turning off video streaming.", m_video_streaming_config->camera);
      m_video_streaming_config->enabled = false;
    }
  }

#pragma endregion
}

void
Configuration::reload()
{
  reload(get_configuration_file_data());
}

DriveMode
string_to_drive_mode(string drive_mode)
{
  std::transform(drive_mode.begin(), drive_mode.end(), drive_mode.begin(),
      [](unsigned char ch) { return std::tolower(ch); });

  if (drive_mode == "mecanum")
    return DriveMode::MECANUM;
  return DriveMode::TANK;
}

toml::table
get_configuration_file_data()
{
  string filepath = get_configuration_file_path();

  toml::table config;
  if (!filepath.empty()) {
    try {
      config = toml::parse_file(filepath);
    } catch (const toml::parse_error& err) {
      std::cerr << err.what() << std::endl;
      std::exit(1);
    }
  } else
    spdlog::warn("Config: WARNING! Configuration file not found! Using default values.");

  return config;
}

string
get_configuration_file_path()
{
  const string paths[] = {
    "/etc/yrosd.conf",
    "/usr/local/etc/yrosd.conf",
    "yrosd.conf",
    "../yrosd.conf",
  };

  string filepath;

  for (auto file : paths) {
    if (std::filesystem::exists(file)) {
      filepath = file;
      break;
    }
  }

  spdlog::info("Config: Config file path found: {}", filepath.empty() ? "NONE" : filepath);

  return filepath;
}

}
