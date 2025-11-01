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
        QList<MultiModalMessage::Part> messagesParts = {{MultiModalMessage::Part::Type::Value::Text, "Test"}, {MultiModalMessage::Part::Type::Value::ImageUrl, "lksdfsfkdsol"}};
        QList<std::shared_ptr<MessageBase>> messages = {std::make_shared<MultiModalMessage>(messagesParts, "user")};

        api.chat(messages);
    });


    return app.exec();
}
