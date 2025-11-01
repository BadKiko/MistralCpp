#include <QImage>
#include <QBuffer>

class MistralUtils
{
public:
    MistralUtils();
    QString qImageToBase64(const QImage &image);
    QString toSnakeCase(QString text);
};
