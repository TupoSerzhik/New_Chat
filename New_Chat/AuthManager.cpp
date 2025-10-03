#include "AuthManager.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>

#ifdef _WIN32
#include <io.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif

AuthManager::AuthManager()
{
    loadUsersFromFile();

    if (users.empty()) {
        users.push_back(User("admin", sha1.hash("admin123")));
        users.push_back(User("user1", sha1.hash("password1")));
        users.push_back(User("user2", sha1.hash("password2")));
        saveUsersToFile();
    }
}

AuthManager::~AuthManager()
{
    saveUsersToFile();
}

std::string AuthManager::hashPassword(const std::string& password) const
{
    return sha1.hash(password);
}

void AuthManager::loadUsersFromFile()
{
    std::ifstream file(userFilename);
    if (!file.is_open()) {
        std::cout << "Файл пользователей не найден, будет создан новый.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            User user = User::deserialize(line);
            users.push_back(user);
        }
    }
    file.close();
}

void AuthManager::saveUsersToFile()
{
    std::ofstream file(userFilename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл для записи пользователей!\n";
        return;
    }

    for (const auto& user : users) {
        file << user.serialize() << "\n";
    }
    file.close();

    setFilePermissions(userFilename);
}

void AuthManager::setFilePermissions(const std::string& filename)
{
#ifdef _WIN32
    _chmod(filename.c_str(), _S_IREAD | _S_IWRITE);
#else
    chmod(filename.c_str(), S_IRUSR | S_IWUSR);
#endif
}

bool AuthManager::registerUser(const std::string& username, const std::string& password)
{
    for (const auto& user : users)
    {
        if (user.getLogin() == username)
        {
            return false;
        }
    }

    users.push_back(User(username, hashPassword(password)));
    saveUsersToFile();
    return true;
}

User* AuthManager::login(const std::string& username, const std::string& password)
{
    std::string hashedPassword = hashPassword(password);
    for (auto& user : users)
    {
        if (user.getLogin() == username && user.getPass() == hashedPassword)
        {
            return &user;
        }
    }
    return nullptr;
}

User* AuthManager::findUser(const std::string& username)
{
    for (auto& user : users)
    {
        if (user.getLogin() == username)
        {
            return &user;
        }
    }
    return nullptr;
}

bool AuthManager::checkUser(const std::string& loginuser)
{
    for (auto& user : users)
    {
        if (user.getLogin() == loginuser)
        {
            return true;
        }
    }
    return false;
}

const std::vector<User>& AuthManager::getUsers() const
{
    return users;
}

void AuthManager::listUsers() const
{
    std::cout << "\n Зарегистрированные пользователи \n";
    for (const auto& user : users)
    {
        std::cout << "- " << user.getLogin() << "\n";
    }
}