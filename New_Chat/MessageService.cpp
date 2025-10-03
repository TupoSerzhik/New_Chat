#include "MessageService.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>

#ifdef _WIN32
#include <io.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

MessageService::MessageService(AuthManager& authManager) : authManager(authManager)
{
    loadMessagesFromFile();
}

MessageService::~MessageService()
{
    saveMessagesToFile();
}

void MessageService::loadMessagesFromFile()
{
    std::ifstream file(messageFilename);
    if (!file.is_open()) {
        std::cout << "Файл сообщений не найден, будет создан новый.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            Message msg = Message::deserialize(line);
            messages.push_back(msg);
        }
    }
    file.close();
}

void MessageService::saveMessagesToFile()
{
    std::ofstream file(messageFilename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл для записи сообщений!\n";
        return;
    }

    for (const auto& msg : messages) {
        file << msg.serialize() << "\n";
    }
    file.close();

    setFilePermissions(messageFilename);
}

void MessageService::setFilePermissions(const std::string& filename)
{
#ifdef _WIN32
    _chmod(filename.c_str(), _S_IREAD | _S_IWRITE);
#else
    chmod(filename.c_str(), S_IRUSR | S_IWUSR);
#endif
}

bool MessageService::sendPrivateMessage(const std::string& sender, const std::string& recipient, const std::string& message)
{
    messages.push_back(Message(message, sender, recipient));
    saveMessagesToFile();
    std::cout << " Сообщение отправлено пользователю '" << recipient << "'\n";
    return true;
}

void MessageService::broadcastMessage(const std::string& sender, const std::string& message)
{
    for (auto& user : authManager.getUsers())
    {
        if (user.getLogin() != sender)
        {
            messages.push_back(Message(message, sender, user.getLogin()));
        }
    }
    saveMessagesToFile();
    std::cout << " Сообщение отправлено всем пользователям\n";
}

std::vector<Message> MessageService::getUserMessages(const std::string& username) const
{
    std::vector<Message> userMessages;
    for (const auto& msg : messages) {
        if (msg.getReceiver() == username || msg.getReceiver() == "ALL") {
            userMessages.push_back(msg);
        }
    }
    return userMessages;
}

void MessageService::clearUserMessages(const std::string& username)
{
    std::vector<Message> remainingMessages;
    for (const auto& msg : messages) {
        if (msg.getReceiver() != username) {
            remainingMessages.push_back(msg);
        }
    }
    messages = remainingMessages;
    saveMessagesToFile();
}
