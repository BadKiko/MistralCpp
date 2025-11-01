#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include "mistralcpp.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    MistralApi api("https://api.mistral.ai/v1", "hDXuWc3NksmZEHGQ7McNnQJOz2ptg06k", "pixtral-large-2411");

    QObject::connect(&api, &MistralApi::responseReady, [](const QString &response){
        qDebug() << "Ответ:" << response;
    });

    QObject::connect(&api, &MistralApi::errorOccurred, [](const QString &err){
        qDebug() << "Ошибка:" << err;
    });

    QTimer::singleShot(0, [&api](){
        // Размер изображения
        int width = 200;
        int height = 200;

        // Создаём QImage с форматом RGB32
        QImage image(width, height, QImage::Format_RGB32);

        // Задаём красный цвет
        QColor red = QColor(255, 0, 0);

        // Заполняем всё изображение красным
        image.fill(red);

        QList<MultiModalMessage::Part> messagesParts = {{MultiModalMessage::Part::Type::Value::Text, "Что на фото?"}, {MultiModalMessage::Part::Type::Value::ImageUrl,nullptr,image}};
        QList<std::shared_ptr<MessageBase>> messages = {std::make_shared<MultiModalMessage>(messagesParts, "user")};

        api.chat(messages);
    });


    return app.exec();
}
