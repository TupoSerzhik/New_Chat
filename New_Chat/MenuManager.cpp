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
        cout << "  ����� ��������� � ���! \n\n";
        cout << "  �������� ������������ : \n";
        cout << "  - admin / admin123 \n";
        cout << "  - user1 / password1 \n";
        cout << "  - user2 / password2 \n\n";

        cout << "  1.  ����������� \n";
        cout << "  2.  ���� \n";
        cout << "  3.  ����� \n\n";
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n";
        cout << "  �������� �����: \n=> ";

        cin >> choise_user_start_menu;
        cin.ignore();

        switch (choise_user_start_menu)
        {
        case '1':
        {
            system("cls");
            cout << "  ������� ��� ������������: ";
            getline(cin, username);
            cout << "  ������� ������: ";
            getline(cin, password);

            if (authManager.registerUser(username, password))
            {
                system("cls");
                cout << "  ����������� �������! \n";
                cin.get();
                system("cls");
            }
            else
            {
                system("cls");
                cout << " ������������ ��� ����������! \n";
                cin.get();
                system("cls");
            }
            break;
        }
        case '2':
        {
            system("cls");

            cout << "  ������� ��� ������������ : ";
            getline(cin, username);
            cout << "  ������� ������: ";
            getline(cin, password);

            currentUser = authManager.login(username, password);
            if (currentUser)
            {
                system("cls");
                cout << " �������, ����� ����������, " << username << "!\n";
                cin.ignore();
                system("cls");

                bool start_sign_menu = true;

                do
                {
                    cout << "=-=-=-=-=-=-= ����� =-=-=-=-=-=-=\n\n";
                    cout << "  ������� ������������: " << currentUser->getLogin() << "\n\n";

                    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
                    cout << "  1.  ��������� ������ ���������  \n";
                    cout << "  2.  ��������� ��������� ���� \n";
                    cout << "  3.  �������� ��� ��������� \n";
                    cout << "  4.  ������ �������������\n";
                    cout << "  5.  �������� ��� ��������� \n";
                    cout << "  6.  ����� �� �������� \n";
                    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n";

                    cout << "  �������� �����: \n=> ";
                    cin >> choise_user_after_sign;

                    switch (choise_user_after_sign)
                    {
                    case '1':
                    {
                        cout << "  ������� ����������: ";
                        cin >> recipient;

                        if (authManager.checkUser(recipient) == false)
                        {
                            cout << "������������ '" << recipient << "' �� ������!" << endl;
                            cin.ignore();
                            system("cls");
                            break;
                        }
                        else
                        {
                            cin.ignore();
                            cout << endl << "  ������� ���������: ";
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

                        cout << "  ������� ��������� ��� ����: \n=> ";
                        cin >> message;

                        messageService.broadcastMessage(currentUser->getLogin(), message);
                        cin.get();
                        system("cls");
                        break;
                    }
                    case '3': 
                    {
                        system("cls");
                        cout << "\n  ���� ���������: \n";
                        auto userMessages = messageService.getUserMessages(currentUser->getLogin());
                        cin.ignore();

                        if (userMessages.empty())
                        {
                            cout << "  � ��� ��� ��������� \n";
                        }
                        else
                        {
                            for (size_t i = 0; i < userMessages.size(); i++)
                            {
                                cout << (i + 1) << ". �� " << userMessages[i].getSender()
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
                        cout << "  �������� ����� ������ ����� ���������� ��� ������ ������... ";
                        cin.ignore();
                        cin.get();
                        system("cls");
                        break;
                    }
                    case '5':
                    {
                        messageService.clearUserMessages(currentUser->getLogin());
                        system("cls");
                        cout << " ��������� ������� \n �������� ����� ������ ����� ���������� ��� ������ ������... ";
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
                cout << "  �������� ������� ������! \n �������� ����� ������ ����� ���������� ��� ������ ������...";
                cin.ignore();
                system("cls");
            }
            system("cls");
            break;
        }
        case '3':
        {
            system("cls");
            cout << "  ����� �� �������... \n �������� ����� ������ ����� �����...";
            start_menu_manager = false;
            cin.ignore();
            system("cls");
            break;
        }
        default:
        {
            system("cls");
            cout << "  �������� �����! \n �������� ����� ������ ����� ���������� ��� ������ ������...";
            cin.ignore();
            system("cls");
        }
        }
    }
    while (start_menu_manager == true);
}