#include<iostream>
#include<windows.h>// для FindWindow GetWindowThreadProcessId ect...
#include<clocale>
#include<conio.h>

int main(){
    // setlocale(LC_ALL, "Russian");
    // // Устанавливаем кодировку ввода (CP_UTF8 или 1251) и вывода консоли
    // SetConsoleCP(1251);
    // SetConsoleOutputCP(1251);
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF8");
    
    const wchar_t* path = L"C:\\Users\\Mason1337\\Desktop\\Game\\build\\Debug\\outDebug.exe";// ПУТЬ ИМЕННО игры o yeah


    HWND hwnd = FindWindowW(NULL, path);// Получаем хэндл окна игры
    if (hwnd == NULL) {
        std::cout<<"[!] Ошибка: Не удалось найти окно игры. Проверь заголовок path!"<<std::endl;
        DWORD errorcode=GetLastError();
        std::cout<<"Ошибка в том что:"<<errorcode<<std::endl;
        std::system("pause");
        return 1;
    }
    std::cout << "[+] Окно игры успешно найдено!" << std::endl;

    DWORD processId = 0;
    GetWindowThreadProcessId(hwnd, &processId);// Получаем ID процесса игры
    if (processId == 0){
        
        DWORD errorcode=GetLastError();
        std::cout<<"Ошибка в том что:"<<errorcode<<std::endl;
        std::system("pause");
        return 1;
    }
    std::cout << "[+] ID Процесса игры (PID): " << processId << std::endl;

    HANDLE permissions=OpenProcess(
        PROCESS_VM_READ|PROCESS_VM_WRITE|PROCESS_VM_OPERATION,//It is DWORD
        FALSE, //IT IS bInheritHandle BOOL
        processId
    );
    if (permissions==NULL){
        std::cout<<"[!] Ошибка: Не удалось получить HANDLE (дескриптор в Kernel32)."<<std::endl;
        DWORD errorcode=GetLastError();
        if (errorcode==5){
            std::cout<<"Запустите игру от имени администратора!"<<std::endl;
        }
        std::cout<<"Ошибка в том что:"<<errorcode<<std::endl;
        std::system("pause");
        return 1;
    }
    std::cout << "[+] Успешно подключились к процессу! Хэндл получен." << std::endl;


    uintptr_t MemoryAddress = 0x005FFE4C;// Мой адресс значения
    int HackableThing=0;
    SIZE_T bytesRead = 0;
    std::cout<<"Начинаем читать адрес:  0x"<<std::hex<<MemoryAddress<<"..."<<std::endl;
    while(true){//Нам нужен цикл чтобы постоянно обновлять данные в нашей переменной так и работает CheatEngine 
        BOOL RPM_Success = ReadProcessMemory(
            permissions,
            (LPCVOID)MemoryAddress,//Начало чтения памяти
            &HackableThing,//буфер в переменную которой будет передаваться то значение по адресу которого я ищу
            sizeof(HackableThing),//сколько надо прочитать начиная с адреса MemoryAccess чтобы получить желаюмую валюту для хака
            &bytesRead// сколько удалось прочитать данных с этого процесса
        );
        if (RPM_Success){
            std::cout<<"\r Здоровье игрока в данном случае:"<<std::dec<<HackableThing<<"  "<<std::flush;//std::dec покажет вывод в 10-ти ричной системе счисления а std::flush мгновенно выведет данные до очистки буфера из за \r переноса каретки в исходное положение 
            
        }else{
            std::cout<<" Ошибка в чтении памяти! Код ошибки:"<<GetLastError()<<std::endl;
            break;
        }
        //Проверка нажали ли кнопку на клавиатуре пользователь(код здесь НЕ останавливается!)
        if(_kbhit()){
            char key = _getch();// x- eXecute своего чита 
            if(key=='x'||key=='X'){
                std::cout << "\n\n[Режим редактирования] ------------------------" << std::endl;
                std::cout << "Введите новое значение здоровья: ";
                int NewHealthValue =0;
                std::cin>>NewHealthValue;

                BOOL WPM = WriteProcessMemory(
                    permissions,
                    LPVOID(MemoryAddress),
                    &NewHealthValue,
                    sizeof(NewHealthValue),
                    NULL
                );
                if (WPM){
                    std::cout<<"[+++]Здоровье поменялось на:"<<std::dec<<NewHealthValue<<std::flush;

                }else{
                    std::cout<<"[-]Ошибочка а именно код:"<<GetLastError()<<std::endl;
                    
                }
                std::cout<<"Возвращаемся в режим мониторинрга\n";
            }
        }
        Sleep(100);//проверка каждые 0.1 сек чтобы не было лагов
    }
    CloseHandle(permissions);//Обязательно чтобы не было утечек памяти это же  индекс в таблице дескрипторов процесса где наша игра и наши значения!
    return 0;
}
