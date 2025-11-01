#include "mistralcpp.h"
#include "utils/mistralutils.h"
#include "QDebug"
/* Constructor */
MessageBase::MessageBase(QString role) : mRole(role) {}

TextMessage::TextMessage(QString content, QString role) : MessageBase(role), mContent(content) {}

MultiModalMessage::MultiModalMessage(QList<Part> content, QString mRole) : MessageBase(mRole), mContent(content) {}
QJsonObject MultiModalMessage::toJson(){
    QJsonArray jsonArr;

    for(const MultiModalMessage::Part &mPart : std::as_const(mContent)){
        QJsonObject obj;
        MistralUtils utils;
        obj["type"] = mPart.type.toString();

        if(mPart.type.getValue() == MultiModalMessage::Part::Type::Text){
            obj["text"] = mPart.text;
        }
        else if(mPart.type.getValue() == MultiModalMessage::Part::Type::ImageUrl){
            if(!mPart.text.isNull()){
                obj["image_url"] = mPart.text;
            }
            else{
                obj["image_url"] = "data:image/jpeg;base64,"+MistralUtils().qImageToBase64(mPart.image);
            }
        }

        jsonArr.append(obj);
    }

    QJsonObject obj;
    obj["role"] = mRole;
    obj["content"] = jsonArr;

    return obj;
}

QJsonObject TextMessage::toJson(){
    QJsonObject obj;
    obj["role"] = mRole;
    obj["content"] = mContent;

    return obj;
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

    for (std::shared_ptr <MessageBase> message : std::as_const(chatHistory)) {
        qJsonMessages.append(message->toJson());
    }

    json["messages"] = qJsonMessages;

    QJsonDocument doc(json);

    qDebug() << doc.toJson();

    QByteArray data = doc.toJson();

    // Отправка POST-запроса
    QNetworkReply *reply = manager->post(request, data);

    // Асинхронная обработка ответа
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QByteArray resp = reply->readAll();
        if (reply->error() == QNetworkReply::NoError) {
            emit responseReady(QString::fromUtf8(resp));
        } else {
            // Выводим текст ответа (если есть) + код ошибки
            QString errMsg = reply->errorString();
            QString respBody = QString::fromUtf8(resp);
            emit errorOccurred(QString("HTTP %1: %2").arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()).arg(respBody.isEmpty() ? errMsg : respBody));
        }
        reply->deleteLater();
    });
}

void MistralApi::resetContext(){
    chatHistory = {};
}
