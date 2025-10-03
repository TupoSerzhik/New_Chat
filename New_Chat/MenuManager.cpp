#include "MenuManager.h"
#include "AuthManager.h"

using namespace std;

void Start_Chating()
{
    setlocale(LC_ALL, "ru");
    AuthManager authManager;
    MessageService messageService(authManager);

    User* currentUser = nullptr;

    string username;
    string password;
    string recipient;
    string message;

    char choise_user_start_menu;
    char choise_user_after_sign;

    bool start_menu_manager = true;

    do
    {
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
        cout << "  Добро пожаловть в чат! \n\n";
        cout << "  Открытые пользователи : \n";
        cout << "  - admin / admin123 \n";
        cout << "  - user1 / password1 \n";
        cout << "  - user2 / password2 \n\n";

        cout << "  1.  Регистрация \n";
        cout << "  2.  Вход \n";
        cout << "  3.  Выход \n\n";
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n";
        cout << "  Выберите опцию: \n=> ";

        cin >> choise_user_start_menu;
        cin.ignore();

        switch (choise_user_start_menu)
        {
        case '1':
        {
            system("cls");
            cout << "  Введите имя пользователя: ";
            getline(cin, username);
            cout << "  Введите пароль: ";
            getline(cin, password);

            if (authManager.registerUser(username, password))
            {
                system("cls");
                cout << "  Регистрация успешна! \n";
                cin.get();
                system("cls");
            }
            else
            {
                system("cls");
                cout << " Пользователь уже существует! \n";
                cin.get();
                system("cls");
            }
            break;
        }
        case '2':
        {
            system("cls");

            cout << "  Введите имя пользователя : ";
            getline(cin, username);
            cout << "  Введите пароль: ";
            getline(cin, password);

            currentUser = authManager.login(username, password);
            if (currentUser)
            {
                system("cls");
                cout << " Успешно, добро пожаловать, " << username << "!\n";
                cin.ignore();
                system("cls");

                bool start_sign_menu = true;

                do
                {
                    cout << "=-=-=-=-=-=-= Опции =-=-=-=-=-=-=\n\n";
                    cout << "  Текущий пользователь: " << currentUser->getLogin() << "\n\n";

                    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
                    cout << "  1.  Отправить личное сообщение  \n";
                    cout << "  2.  Отправить сообщение всем \n";
                    cout << "  3.  Показать мои сообщения \n";
                    cout << "  4.  Список пользователей\n";
                    cout << "  5.  Очистить мои сообщения \n";
                    cout << "  6.  Выйти из аккаунта \n";
                    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n";

                    cout << "  Выберите опцию: \n=> ";
                    cin >> choise_user_after_sign;

                    switch (choise_user_after_sign)
                    {
                    case '1':
                    {
                        cout << "  Введите получателя: ";
                        cin >> recipient;

                        if (authManager.checkUser(recipient) == false)
                        {
                            cout << "Пользователь '" << recipient << "' не найден!" << endl;
                            cin.ignore();
                            system("cls");
                            break;
                        }
                        else
                        {
                            cin.ignore();
                            cout << endl << "  Введите сообщение: ";
                            getline(cin, message);

                            messageService.sendPrivateMessage(currentUser->getLogin(), recipient, message);
                            cin.ignore();
                            cin.get();
                            system("cls");
                            break;
                        }
                    }
                    case '2':
                    {
                        system("cls");

                        cout << "  Введите сообщение для всех: \n=> ";
                        cin >> message;

                        messageService.broadcastMessage(currentUser->getLogin(), message);
                        cin.get();
                        system("cls");
                        break;
                    }
                    case '3': 
                    {
                        system("cls");
                        cout << "\n  Ваши сообщения: \n";
                        auto userMessages = messageService.getUserMessages(currentUser->getLogin());
                        cin.ignore();

                        if (userMessages.empty())
                        {
                            cout << "  У вас нет сообщений \n";
                        }
                        else
                        {
                            for (size_t i = 0; i < userMessages.size(); i++)
                            {
                                cout << (i + 1) << ". От " << userMessages[i].getSender()
                                    << ": " << userMessages[i].getText() << "\n";
                            }
                        }
                        cin.get();
                        system("cls");
                        break;
                    }
                    case '4':
                    {
                        system("cls");
                        authManager.listUsers();
                        cout << "  Нажимите любую кнопку чтобы продолжить или начать заново... ";
                        cin.ignore();
                        cin.get();
                        system("cls");
                        break;
                    }
                    case '5':
                    {
                        messageService.clearUserMessages(currentUser->getLogin());
                        system("cls");
                        cout << " Сообщения очищены \n Нажимите любую кнопку чтобы продолжить или начать заново... ";
                        cin.ignore();
                        cin.get();
                        system("cls");
                        break;
                    }
                    case '6':
                    {
                        start_sign_menu = false;
                        cin.ignore();
                        system("cls");
                        break;
                    }
                    }
                } while (start_sign_menu == true);
            }
            else
            {
                system("cls");
                cout << "  Неверные учетные данные! \n Нажимите любую кнопку чтобы продолжить или начать заново...";
                cin.ignore();
                system("cls");
            }
            system("cls");
            break;
        }
        case '3':
        {
            system("cls");
            cout << "  Выход из системы... \n Нажимите любую кнопку чтобы выйти...";
            start_menu_manager = false;
            cin.ignore();
            system("cls");
            break;
        }
        default:
        {
            system("cls");
            cout << "  Неверный выбор! \n Нажимите любую кнопку чтобы продолжить или начать заново...";
            cin.ignore();
            system("cls");
        }
        }
    }
    while (start_menu_manager == true);
}