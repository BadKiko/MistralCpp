#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QImage>
#include <QBuffer>

class MessageBase {
    public:
        MessageBase(QString mRole);
        virtual QJsonObject toJson() = 0;
        virtual ~MessageBase() = default;

        QString mRole;
};

class TextMessage : public MessageBase {
    public:
        TextMessage(QString content, QString role);
        QJsonObject toJson() override;

    private:
        QString mContent;

};

class MultiModalMessage : public MessageBase {
    
    public:
        struct Part {
            class Type {
            public:
                enum Value { Text, ImageUrl };

                Type(Value v) : value(v) {}

                QString toString() const {
                    switch (value) {
                    case Text: return "text";
                    case ImageUrl: return "image_url";
                    default: return "unknown";
                    }
                }

                Value getValue() const {return value;}

            private:
                Value value;
            };


            Type type;
            QString text;
            QImage image;
        };
        MultiModalMessage(QList<Part> content, QString role);
        QJsonObject toJson() override;

    private:
        QList<Part> mContent;

};

class MistralApi : public QObject
{
    Q_OBJECT
public:
    explicit MistralApi(const QString &mistralUrl, const QString &mistralToken, const QString &model, QObject *parent = nullptr);

    Q_INVOKABLE void chat(const QList<std::shared_ptr<MessageBase>> messages);
    Q_INVOKABLE void resetContext();

signals:
    void responseReady(const QString &response);
    void errorOccurred(const QString &message);

private:
    std::shared_ptr<QNetworkAccessManager> manager;
    QList<std::shared_ptr<MessageBase>> chatHistory;
    QString rMistralUrl;
    QString rMistralToken;
    QString rMistralModel;
};
