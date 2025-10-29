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
        QString role;
        MessageBase(QString mRole){role = mRole;}
        virtual QString getContent();
        virtual ~MessageBase() = default;
};

class TextMessage : public MessageBase {
    public:
        QString getContent() override;
        TextMessage(QString content, QString mRole) : MessageBase(mRole){
            mContent = content;
        }

    private:
        QString mContent;

};

class MultiModalMessage : public MessageBase {
    struct Part {
        enum class Type { Text, ImageUrl } type;
        QString text;
        QString imageUrl;
    };

    public:
        QString getContent() override;
        MultiModalMessage(Part content, QString mRole) : MessageBase(mRole){
            content = {content};
        }

    private:
        QList<Part> content;

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
