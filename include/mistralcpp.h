#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

class MessageBase {
    public:
        MessageBase(QString mRole);
        virtual QString getContent() = 0;
        virtual QString getRole();
        virtual ~MessageBase() = default;
    
    private:
        QString role;
};

class TextMessage : public MessageBase {
    public:
        TextMessage(QString content, QString mRole);    
        QString getContent() override;

    private:
        QString mContent;

};

class MultiModalMessage : public MessageBase {
    
    public:
        struct Part {
            enum class Type { Text, ImageUrl } type;
            QString text;
            QString imageUrl;
        };
        MultiModalMessage(Part content, QString mRole);
        QString getContent() override;

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
