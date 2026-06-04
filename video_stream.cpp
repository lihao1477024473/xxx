#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

/**
 * 视频拉流函数 - 支持本地文件、HTTP/HTTPS URL、RTSP/RTMP 流
 *
 * @param source        视频源路径（本地文件 / HTTP / RTSP / RTMP）
 * @param windowName    显示窗口名称，默认 "Video"
 * @param maxRetries    最大重试次数，默认 3
 * @param retryDelaySec 重试间隔（秒），默认 2
 * @return true 成功，false 失败
 */
bool pullVideo(const std::string &source,
               const std::string &windowName = "Video",
               int maxRetries = 3,
               int retryDelaySec = 2)
{
    cv::VideoCapture cap;

    for (int attempt = 1; attempt <= maxRetries; ++attempt)
    {
        std::cout << "尝试连接视频源 (第 " << attempt << " 次)..." << std::endl;
        cap.open(source);

        if (cap.isOpened())
        {
            std::cout << "连接成功！" << std::endl;
            break;
        }
        else
        {
            cap.release();
            std::cout << "连接失败，" << retryDelaySec << " 秒后重试..." << std::endl;
            if (attempt < maxRetries)
            {
                std::this_thread::sleep_for(std::chrono::seconds(retryDelaySec));
            }
            else
            {
                std::cerr << "已达到最大重试次数，无法连接视频源。" << std::endl;
                return false;
            }
        }
    }

    if (!cap.isOpened())
    {
        std::cerr << "无法打开视频源。" << std::endl;
        return false;
    }

    cv::namedWindow(windowName, cv::WINDOW_NORMAL);

    int frameCount = 0;
    auto startTime = std::chrono::steady_clock::now();

    while (true)
    {
        cv::Mat frame;
        bool ret = cap.read(frame);

        if (!ret)
        {
            std::cout << "视频流结束或读取失败。" << std::endl;
            break;
        }

        ++frameCount;
        auto elapsed = std::chrono::steady_clock::now() - startTime;
        double elapsedSec = std::chrono::duration<double>(elapsed).count();
        double fps = (elapsedSec > 0) ? (frameCount / elapsedSec) : 0.0;

        // 在画面上显示帧率和帧数
        std::string infoText = "FPS: " + std::to_string(fps).substr(0, 5) +
                               " | Frames: " + std::to_string(frameCount);
        cv::putText(frame, infoText, cv::Point(10, 30),
                    cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);

        cv::imshow(windowName, frame);

        // 按 ESC 或 'q' 键退出
        int key = cv::waitKey(1) & 0xFF;
        if (key == 27 || key == 'q')
        {
            std::cout << "用户退出。" << std::endl;
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();

    auto totalElapsed = std::chrono::steady_clock::now() - startTime;
    double totalSec = std::chrono::duration<double>(totalElapsed).count();
    std::cout << "共读取 " << frameCount << " 帧，耗时 "
              << totalSec << " 秒，平均 "
              << (totalSec > 0 ? frameCount / totalSec : 0.0) << " FPS" << std::endl;

    return true;
}

/**
 * 拉取视频流并保存到本地文件
 *
 * @param source        视频源路径
 * @param outputPath    输出文件保存路径
 * @param maxRetries    最大重试次数
 * @param retryDelaySec 重试间隔（秒）
 * @return true 成功，false 失败
 */
bool pullVideoToFile(const std::string &source,
                     const std::string &outputPath,
                     int maxRetries = 3,
                     int retryDelaySec = 2)
{
    cv::VideoCapture cap;

    for (int attempt = 1; attempt <= maxRetries; ++attempt)
    {
        std::cout << "尝试连接视频源 (第 " << attempt << " 次)..." << std::endl;
        cap.open(source);

        if (cap.isOpened())
        {
            std::cout << "连接成功！" << std::endl;
            break;
        }
        else
        {
            cap.release();
            if (attempt < maxRetries)
            {
                std::this_thread::sleep_for(std::chrono::seconds(retryDelaySec));
            }
            else
            {
                std::cerr << "已达到最大重试次数，无法连接视频源。" << std::endl;
                return false;
            }
        }
    }

    if (!cap.isOpened())
    {
        std::cerr << "无法打开视频源。" << std::endl;
        return false;
    }

    // 获取视频属性
    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(cv::CAP_PROP_FPS);
    if (fps <= 0)
        fps = 25.0;

    int fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
    cv::VideoWriter writer(outputPath, fourcc, fps, cv::Size(width, height));

    if (!writer.isOpened())
    {
        std::cerr << "无法创建视频写入器。" << std::endl;
        cap.release();
        return false;
    }

    int frameCount = 0;
    try
    {
        while (true)
        {
            cv::Mat frame;
            bool ret = cap.read(frame);
            if (!ret)
                break;

            writer.write(frame);
            ++frameCount;

            // 每 100 帧打印一次进度
            if (frameCount % 100 == 0)
            {
                std::cout << "已保存 " << frameCount << " 帧..." << std::endl;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "保存过程中发生错误: " << e.what() << std::endl;
        cap.release();
        writer.release();
        return false;
    }

    cap.release();
    writer.release();

    std::cout << "视频保存完成，共 " << frameCount << " 帧，路径: " << outputPath << std::endl;
    return true;
}

int main()
{
    // 示例 1: 拉取本地视频并播放
    // pullVideo("video.mp4");

    // 示例 2: 拉取 RTSP 流并播放
    // pullVideo("rtsp://admin:password@192.168.1.100:554/stream1");

    // 示例 3: 拉取网络视频并保存到本地
    // pullVideoToFile("rtsp://admin:password@192.168.1.100:554/stream1", "output.mp4");

    std::cout << "请取消注释上方示例代码来测试视频拉流功能。" << std::endl;
    std::cout << "支持的协议: 本地文件, HTTP/HTTPS, RTSP, RTMP" << std::endl;
    return 0;
}
