#pragma once

#include <Common.h>
#include <unistd.h>
#include <toml++/impl/table.h>

namespace Config {

enum DriveMode {
    TANK,
    MECANUM,
};

#pragma region Configuration file structs

struct Server {
    string host = "0.0.0.0";

    int port = 9080;
    int port_udp = 8092;
};

struct Motors {
    bool enabled = false;

    int back_left[4] = { 12, 5, 22, 24 };
    int back_right[4] = { 13, 6 , 23, 25};
    int front_left[4] = { 0 };
    int front_right[4] = { 0 };

    bool left_inverted = true;
    bool right_inverted = true;

    DriveMode drive_mode = DriveMode::TANK;
};

struct Head {
    bool enabled = false;

    int yaw = 0;
    int pitch = 1;

    int init_yaw = 90;
    int init_pitch = 90;
};

struct Hand {
    bool enabled = false;

    int left = 14;
    int right = 15;

    int open_angle = 180;
    int close_angle = 0;
};

struct Wrtists {
    bool enabled = false;

    int left = 13;
    int right = 12;

    int max_angle = 180;
    int min_angle = 0;
};

struct FaceTracking {
    bool enabled = false;

    int head_camera = 0;
    int rotate_camera = 90;

    int speed_x = 3;
    int speed_y = 2;
};

struct Face {
    bool emotions = false;
};

struct VideoStreaming {
    bool enabled = false;

    int port = 9081;
    int camera = 0;

    int width = 640;
    int height = 360;

    int framerate = 30;
    int jpeg_quality = 50;
};

#pragma endregion

class Configuration
{
public:
    Configuration(toml::table toml_data);

    void reload(toml::table toml_data);
    void reload();

    Server* server() const { return m_server_config; }    
    Motors* motors() const { return m_motors_config; }    
    Head* head() const { return m_head_config; }    
    Hand* hand() const { return m_hand_config; }    
    Wrtists* wrists() const { return m_wrists_config; }    
    FaceTracking* face_tracking() const { return m_face_tracking_config; }    
    Face* face() const { return m_face_config; }    
    VideoStreaming* video_streaming() const { return m_video_streaming_config; }    

    bool gui_present() const { return m_gui_present; }    

    string as_printable_string();

    static Configuration& the();

private:
    Server *m_server_config = new Server;
    Motors *m_motors_config = new Motors;
    Head *m_head_config = new Head;
    Hand *m_hand_config = new Hand;
    Wrtists *m_wrists_config = new Wrtists;
    FaceTracking *m_face_tracking_config = new FaceTracking;
    Face *m_face_config = new Face;
    VideoStreaming *m_video_streaming_config = new VideoStreaming;

    bool m_gui_present = false;
};

}

using Config::Configuration;

