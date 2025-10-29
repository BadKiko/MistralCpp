#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include "mistralcpp.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    MistralApi api("https://api.mistral.ai/v1", "hDXuWc3NksmZEHGQ7McNnQJOz2ptg06k", "ministral-3b-2410");

    QObject::connect(&api, &MistralApi::responseReady, [](const QString &response){
        qDebug() << "Ответ:" << response;
    });

    QObject::connect(&api, &MistralApi::errorOccurred, [](const QString &err){
        qDebug() << "Ошибка:" << err;
    });

    QTimer::singleShot(0, [&api](){
        QList<std::shared_ptr<MessageBase>> messages = {std::make_shared<TextMessage>("penis", "user")};

        api.chat(messages);
    });


    return app.exec();
}
