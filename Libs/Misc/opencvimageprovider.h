#pragma once
#include <QQuickImageProvider>
#include <QImage>
#include <QMutex>
#include <opencv2/opencv.hpp>

class OpenCVImageProvider : public QQuickImageProvider
{
public:
    OpenCVImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

    // Метод, который вызовет QML для получения картинки
     QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override
    {
       int width = 5120;
       int height = 3958;
               if (size)
          *size = QSize(width, height);
       QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
                      requestedSize.height() > 0 ? requestedSize.height() : height);
       pixmap.fill(QColor(id).rgba());
       return pixmap;
    }

    // Метод для обновления кадра из вашего BarcodeAnalyzer
    void updateImage(const cv::Mat &mat) {
        QMutexLocker locker(&m_mutex);
        if (mat.empty()) return;

        // Конвертируем cv::Mat (BGR) в QImage (RGB) без копирования памяти
        cv::Mat temp;
        cv::cvtColor(mat, temp, cv::COLOR_BGR2RGB);
        m_currentImage = QImage(temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888).copy(); 
    }

private:
    QImage m_currentImage;
    QMutex m_mutex; // Защита от многопоточных коллизий
};
