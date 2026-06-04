# 视频拉流工具 (Video Stream Puller)

一个基于 OpenCV 的 C++ 视频拉流工具，支持从多种视频源拉取视频流，支持实时播放和保存到本地文件。

## 功能特性

- 支持多种视频源：本地文件、HTTP/HTTPS URL、RTSP 流、RTMP 流
- 自动重连机制（可配置重试次数和间隔）
- 实时显示 FPS 和帧数
- 支持实时播放和保存到文件两种模式
- 按 `q` 或 `ESC` 键退出播放

## 环境要求

- C++11 或更高版本
- [OpenCV](https://opencv.org/) 4.x（需包含 videoio、highgui、imgproc 模块）
- CMake 3.10+（推荐）

## 编译

### 使用 g++ (Linux / MinGW)

```bash
g++ -std=c++11 -O2 video_stream.cpp -o video_stream \
    $(pkg-config --cflags --libs opencv4)
```

### 使用 CMake

创建 `CMakeLists.txt`：

```cmake
cmake_minimum_required(VERSION 3.10)
project(VideoStreamPuller)
find_package(OpenCV REQUIRED)
add_executable(video_stream video_stream.cpp)
target_link_libraries(video_stream ${OpenCV_LIBS})
```

然后编译：

```bash
mkdir build && cd build
cmake ..
make
```

### Windows (MSVC)

```bash
cl /EHsc /std:c++11 video_stream.cpp /I <opencv_include_path> /link <opencv_libs>
```

或在 Visual Studio 中配置 OpenCV 后直接编译。

## 使用说明

### 1. 拉取视频并实时播放

```cpp
#include "video_stream.cpp"

// 播放本地视频文件
pullVideo("video.mp4");

// 播放 RTSP 摄像头流
pullVideo("rtsp://admin:password@192.168.1.100:554/stream1");

// 自定义窗口名称和重试策略
pullVideo("rtmp://example.com/live/stream", "监控画面", 5, 3);
```

### 2. 拉取视频并保存到文件

```cpp
// 将 RTSP 流保存为本地 MP4 文件
pullVideoToFile("rtsp://admin:password@192.168.1.100:554/stream1", "output.mp4");
```

## API 参考

### `pullVideo()`

```cpp
bool pullVideo(
    const std::string &source,         // 视频源路径
    const std::string &windowName = "Video",  // 窗口标题
    int maxRetries = 3,                // 最大重试次数
    int retryDelaySec = 2              // 重试间隔（秒）
);
```

| 参数 | 说明 |
|------|------|
| `source` | 视频源，支持本地路径、HTTP/HTTPS URL、RTSP、RTMP |
| `windowName` | OpenCV 显示窗口的名称 |
| `maxRetries` | 连接失败时的最大重试次数 |
| `retryDelaySec` | 每次重试之间的等待秒数 |

**返回值**：`true` 成功，`false` 失败

### `pullVideoToFile()`

```cpp
bool pullVideoToFile(
    const std::string &source,         // 视频源路径
    const std::string &outputPath,     // 输出文件路径
    int maxRetries = 3,                // 最大重试次数
    int retryDelaySec = 2              // 重试间隔（秒）
);
```

| 参数 | 说明 |
|------|------|
| `source` | 视频源，支持本地路径、HTTP/HTTPS URL、RTSP、RTMP |
| `outputPath` | 保存的视频文件路径（MP4 格式） |
| `maxRetries` | 连接失败时的最大重试次数 |
| `retryDelaySec` | 每次重试之间的等待秒数 |

**返回值**：`true` 成功，`false` 失败

## 支持的视频源格式

| 协议 | 示例 |
|------|------|
| 本地文件 | `video.mp4`、`C:\videos\demo.avi` |
| HTTP/HTTPS | `http://example.com/video.mp4` |
| RTSP | `rtsp://admin:password@192.168.1.100:554/stream1` |
| RTMP | `rtmp://example.com/live/stream` |

## 快捷键

| 按键 | 功能 |
|------|------|
| `q` | 退出播放 |
| `ESC` | 退出播放 |
