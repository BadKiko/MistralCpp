# MistralCpp

Qt-библиотека для удобной работы с API Mistral AI. Поддерживает текстовые и мультимодальные сообщения, асинхронные запросы и управление контекстом чата.

## ✨ Возможности

- **Простой API**: Легко интегрируется в Qt-приложения
- **Асинхронные запросы**: Не блокирует основной поток
- **Мультимодальность**: Поддержка текста и изображений
- **Управление контекстом**: Сохранение истории сообщений
- **Qt-сигналы**: Удобная обработка ответов и ошибок

## 🚀 Быстрый старт

### Установка

```bash
# Клонируйте репозиторий
git clone https://github.com/your-repo/MistralCpp.git
cd MistralCpp

# Соберите проект
mkdir build && cd build
cmake ..
make
```

### Использование

```cpp
#include "mistralcpp.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    // Инициализация API
    MistralApi api("https://api.mistral.ai/v1",
                   "your-api-token",
                   "ministral-3b-2410");

    // Обработка ответов
    QObject::connect(&api, &MistralApi::responseReady,
        [](const QString &response) {
            qDebug() << "Ответ:" << response;
        });

    // Отправка сообщения
    QList<std::shared_ptr<MessageBase>> messages = {
        std::make_shared<TextMessage>("Привет!", "user")
    };
    api.chat(messages);

    return app.exec();
}
```

## 📋 Требования

- Qt 6.x
- CMake 3.16+
- C++17

## 🏗️ Сборка

```bash
# Сборка библиотеки
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make

# Сборка с тестами
make MistralCppTest
```

## 📖 API Reference

### MistralApi

Основной класс для работы с Mistral AI.

```cpp
MistralApi(const QString &url, const QString &token, const QString &model, QObject *parent = nullptr);
```

#### Методы

- `void chat(const QList<std::shared_ptr<MessageBase>> messages)` - Отправить сообщения
- `void resetContext()` - Очистить историю чата

#### Сигналы

- `responseReady(const QString &response)` - Получен ответ от API
- `errorOccurred(const QString &message)` - Произошла ошибка

### Сообщения

#### TextMessage
```cpp
auto message = std::make_shared<TextMessage>("Текст сообщения", "user");
```

#### MultiModalMessage
```cpp
MultiModalMessage::Part part;
part.type = MultiModalMessage::Part::Type::ImageUrl;
part.imageUrl = "https://example.com/image.jpg";

auto message = std::make_shared<MultiModalMessage>(part, "user");
```

## 📄 Лицензия

MIT License
