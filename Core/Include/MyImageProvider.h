#pragma once
#include <QImage>
#include <QQuickImageProvider>
#include <mutex>


#include "MyCamera.h"

class MyImageProvider : public QQuickImageProvider {
  public:
    // Передаем указатель на камеру в конструктор
    explicit MyImageProvider(MyCamera *camera) : QQuickImageProvider(QQuickImageProvider::Image), m_camera(camera) {
#if debugprintting
        printf("constructor of MyImageProvider is called, m_camera: %p", m_camera);
#endif
    }

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override {
        Q_UNUSED(id);
#if debugprintting
        printf("MyImageProvider::requestImage is called");
#endif
        std::lock_guard<std::mutex> lock(mtx);

        if (!m_camera) {
            return QImage();
        }

        
        QImage img = m_camera->getCurrentImage();
        
        if (size) {
            *size = img.size();
        }
        if (requestedSize.isValid()) {
            return img.scaled(requestedSize, Qt::KeepAspectRatio);
        }

        return img;
    }

    
    

  private:
    std::mutex mtx;
    MyCamera *m_camera; // Ссылка на наш класс камеры
};
