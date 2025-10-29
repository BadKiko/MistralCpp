#include "mistralcpp.h"

/* Constructor */
MessageBase::MessageBase(QString mRole) : role(mRole) {}

TextMessage::TextMessage(QString content, QString mRole) : MessageBase(mRole), mContent(content) {}

MultiModalMessage::MultiModalMessage(Part content, QString mRole) : MessageBase(mRole) {
    mContent = {content};
}

/*  Method */
QString MessageBase::getRole() {
    return this->role;
}

QString MultiModalMessage::getContent(){
    QStringList contentList;
    for (const auto& part : mContent) {
        if (part.type == Part::Type::Text) {
            contentList.append(part.text);
        } else if (part.type == Part::Type::ImageUrl) {
            contentList.append(part.imageUrl);
        }
    }
    return contentList.join(", ");
}

QString TextMessage::getContent(){
    return mContent;
}

MistralApi::MistralApi(const QString &mistralUrl, const QString &mistralToken, const QString &model, QObject *parent)
    : QObject(parent), rMistralUrl(mistralUrl), rMistralToken(mistralToken), rMistralModel(model)
{
    manager = std::make_shared<QNetworkAccessManager>(this);
}

void MistralApi::chat(const QList<std::shared_ptr<MessageBase>> messages)
{
    QString chatUrl = rMistralUrl + "/chat/completions";

    QUrl url(chatUrl);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QString authHeader = "Bearer " + rMistralToken;
    request.setRawHeader("Authorization", authHeader.toUtf8());

    // Формируем JSON тело запроса
    QJsonObject json;
    json["model"] = rMistralModel;

    chatHistory.append(messages);

    QJsonArray qJsonMessages;

    for (std::shared_ptr <MessageBase> message : chatHistory) {
        QJsonObject sysMsg;
        sysMsg["role"] = message->getRole();
        sysMsg["content"] = message->getContent();
        qJsonMessages.append(sysMsg);
    }

    json["messages"] = qJsonMessages;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    // Отправка POST-запроса
    QNetworkReply *reply = manager->post(request, data);

    // Асинхронная обработка ответа
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray resp = reply->readAll();
            emit responseReady(QString::fromUtf8(resp));
        } else {
            emit errorOccurred(reply->errorString());
        }
        reply->deleteLater();
    });
}

void MistralApi::resetContext(){
    chatHistory = {};
}
