#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <vector>
using namespace std;

const int Colors = system("color B0");
const int consol = system("mode con cols=80 lines=25");

HANDLE colors;

void gotoxy(int x, int y)
{
    COORD p = { x + 23, y + 2 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
};



const int n = 10; // глобальная константа, которая будет видеться во всем файле
const int num_ships = 10;

int ship_id = 1;
int ships[10] = { 0 };


// функция на росставления кораблей
void rand_ships(int map[n][n], int size, int ship_id)
{
    (srand(time(NULL)));

    int x, y;

    int dir = 0; // переменая случайного направления

    int count = 0;

    int tact = 0; // предохраняющий счетчик 


    while (count < 1) // пока число кораблей не достигнет заданого числа
    {
        tact++;

        if (tact > 1000) // проверка на зацыкления
        {
            break; // принудительная остановка
        }

        // случайная позиция
        x = rand() % n;
        y = rand() % n;

        int temp_x = x;
        int temp_y = y;

        // генерация позиции
        dir = rand() % 4;


        bool possible = 1;
        // проверка на кординаты корабля
        for (int i = 0; i < size; i++) // создание size палубного корабля
        {
            if (x < 0 || y < 0 || x >= n || y >= n) // условие на проверку выхода за границы корабля
            {
                possible = 0;
                break;
            }

            if (map[x][y] >= 1 || map[x][y + 1] >= 1 || map[x][y - 1] >= 1 || map[x + 1][y] >= 1 || map[x + 1][y + 1] >= 1 || map[x + 1][y - 1] >= 1 || map[x - 1][y] >= 1 || map[x - 1][y + 1] >= 1 || map[x - 1][y - 1] >= 1) // проверка на ячеек вокруг 
            {
                possible = 0;
                break;
            }

            switch (dir) // по случайному направлению заполняеться массив
            {
            case 0:
                x++;
                break;

            case 1:
                y++;
                break;

            case 2:
                x--;
                break;

            case 3:
                y--;
                break;
            }
        }


        // если есть возможность поставить корабль - ставим
        if (possible == 1)
        {
            x = temp_x;
            y = temp_y;

            for (int i = 0; i < size; i++)
            {
                map[x][y] = ship_id; // кажному кораблю присваиваеться номер


                switch (dir)
                {
                case 0:
                    x++;
                    break;

                case 1:
                    y++;
                    break;

                case 2:
                    x--;
                    break;

                case 3:
                    y--;
                    break;
                }
            }
            count++; // счетчик удачно поствленых кораблей
        }
    }
}


void map_show(int map[n][n], int mask[n][n], bool use_mask)
{
    char symb = 65;

    // прорисовка поля
    cout << ' ' << ' ';

    for (int i = 0; i < n; i++)
    {
        cout << i << " ";
    }

    cout << endl;

    for (int i = 0; i < n; i++)
    {
        cout << i << " ";

        for (int j = 0; j < n; j++)
        {
            if (mask[j][i] == 1 || use_mask == 0) // маска поля
            {
                if (map[j][i] == 0)
                {
                    cout << '-' << " ";
                }
                else if (map[j][i] == 10)
                {
                    cout << symb << " ";
                }
                else if (map[j][i] == -1)
                {
                    cout << 'X' << " ";
                }
                else if (map[j][i] == -2)
                {
                    cout << '*' << " ";
                }
                else
                {
                    cout << map[j][i] << " ";
                }
            }
            else // маска поля
            {
                cout << ' ' << " ";
            }
        }
        cout << endl;
    }
}


int shot(int map[n][n], int x, int y, int ships[num_ships + 1], int mask[n][n])
{
    int result = 0; // переменая которая будет принимать состояние кораблей 

    if (map[x][y] == -1 || map[x][y] == -2)
    {
        result = 3;
    }

    else if (map[x][y] >= 1) // если в ячейке масива число больше 1 значет там есть палуба корабля
    {
        ships[map[x][y]]--;
        if (ships[map[x][y]] <= 0)
        {
            result = 2; // это будет означать что корабль убит
        }
        else
        {
            result = 1; // это ранен
        }

        map[x][y] = -1; // помечается как -1 что бы правильно отбражать подбитые палубы
    }

    else
    {
        map[x][y] = -2; // промах
    }

    mask[x][y] = 1;

    return result;
}

void gotovo()
{

    const int num = 4;

    int act = 0;

    int che = 0;

    bool gotovo = false;
    cout << "\n\t\t";
    cout << "9.Готово" << endl;

    while (!gotovo)
    {
        gotoxy(0, act);

        che = _getch();
        if (che == 224)
            che = _getch();

        switch (che)
        {
        case 27: gotovo = true; break;   //27
        case 119: act--; break; // w вверх //119
        case 115: act++; break; // s вниз  //115
        case 13:

            if (act == 0)
            {
                gotovo = true;
            }
            break;

        }

        if (act < 0) act = 0;
        if (act > num - 1) act = num - 1;
    }
}


void human_game()
{
    setlocale(LC_ALL, "Russian");


    const int n = 10;

    HANDLE p = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(p, 6 | FOREGROUND_INTENSITY);

    // поля для человека и бота
    int map[n][n] = { 0 };
    int map2[n][n] = { 0 };

    // маска для туманой игры
    int mask[n][n] = { 0 };
    int mask2[n][n] = { 0 };


    int ships1[num_ships + 2] = { 0, 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
    int ships2[num_ships + 2] = { 0, 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

    /*
    for (int i = 0; i <= num_ships; i++)
    {
        rand_ships(map, ships1[i], i);
    }


    for (int i = 0; i <= num_ships; i++)
    {
        rand_ships(map2, ships2[i], i);
    }
    */
    cout << "_______Поле Человека1_________" << endl;
    map_show(map, mask, 0);

    cout << endl;

    cout << "_______Поле Человека2_________" << endl;
    map_show(map2, mask2, 1);

    /*
    int go = 0;
    cout << "\n\t\t";
    gotoxy(0, 2);
    cout << "9.Готово" << endl;
    */


    bool prov = false;
    bool provv = false;

    const int num = 3;

    int act = 0;

    int che = 0;

    bool gotovo = false;

    cout << "\n\n\t\t";
    cout << " Случайная расстановка ";
    cout << "\n\t\t";
    SetConsoleTextAttribute(p, 8 | FOREGROUND_INTENSITY);
    cout << " Расстановка в ручную ";
    cout << "\n\t\t";
    SetConsoleTextAttribute(p, 6 | FOREGROUND_INTENSITY);
    cout << " Готово" << endl;

    while (!gotovo)
    {
        gotoxy(-6, act + 25);

        che = _getch();
        if (che == 224)
            che = _getch();

        switch (che)
        {
        case 27: gotovo = true; break;   //27
        case 119: act--; break; // w вверх //119
        case 115: act++; break; // s вниз  //115
        case 13:

            if (act == 0)
            {

                system("cls");
                // srand(time(0));

                for (int i = 0; i < n; i++)
                {
                    for (int j = 0; j < n; j++)
                    {

                        if (map[j][i] == 0)
                        {
                            map[j][i] = 0;
                        }
                        else if (map[j][i] == -1)
                        {
                            map[j][i] = 0;
                        }
                        else if (map[j][i] == -2)
                        {
                            map[j][i] = 0;
                        }
                        else
                        {
                            map[j][i] = 0;
                        }
                    }
                }


                for (int i = 0; i <= num_ships; i++)
                {
                    rand_ships(map, ships1[i], i);
                }

                prov = true;

                cout << "_______Поле Человека1_________" << endl;
                map_show(map, mask, 0);

                cout << endl;

                cout << "_______Поле Человека2_________" << endl;
                map_show(map2, mask2, 1);


                cout << "\n\n\t\t";
                cout << " Случайная расстановка ";
                cout << "\n\t\t";
                SetConsoleTextAttribute(p, 8 | FOREGROUND_INTENSITY);
                cout << " Расстановка в ручную ";
                cout << "\n\t\t";
                SetConsoleTextAttribute(p, 6 | FOREGROUND_INTENSITY);
                cout << " Готово" << endl;

            }

            else if (act == 2 && prov == true)
            {


                system("cls");


                cout << "_______Поле Человека1_________" << endl;
                map_show(map, mask, 1);

                cout << endl;

                cout << "_______Поле Человека2_________" << endl;
                map_show(map2, mask2, 0);



                cout << "\n\n\t\t";
                cout << " Случайная расстановка ";
                cout << "\n\t\t";
                SetConsoleTextAttribute(p, 8 | FOREGROUND_INTENSITY);
                cout << " Расстановка в ручную ";
                cout << "\n\t\t";
                SetConsoleTextAttribute(p, 6 | FOREGROUND_INTENSITY);
                cout << " Готово" << endl;

                while (!gotovo)
                {
                    gotoxy(-6, act + 25);

                    che = _getch();
                    if (che == 224)
                        che = _getch();

                    switch (che)
                    {
                    case 27: gotovo = true; break;   //27
                    case 119: act--; break; // w вверх //119
                    case 115: act++; break; // s вниз  //115
                    case 13:

                        if (act == 0)
                        {

                            system("cls");
                            // srand(time(0));

                            for (int i = 0; i < n; i++)
                            {
                                for (int j = 0; j < n; j++)
                                {

                                    if (map2[j][i] == 0)
                                    {
                                        map2[j][i] = 0;
                                    }
                                    else if (map2[j][i] == -1)
                                    {
                                        map2[j][i] = 0;
                                    }
                                    else if (map2[j][i] == -2)
                                    {
                                        map2[j][i] = 0;
                                    }
                                    else
                                    {
                                        map2[j][i] = 0;
                                    }
                                }
                            }


                            for (int i = 0; i <= num_ships; i++)
                            {
                                rand_ships(map2, ships2[i], i);
                            }

                            provv = true;

                            cout << "_______Поле Человека1_________" << endl;
                            map_show(map, mask, 1);

                            cout << endl;

                            cout << "_______Поле Человека2_________" << endl;
                            map_show(map2, mask2, 0);


                            cout << "\n\n\t\t";
                            cout << " Случайная расстановка ";
                            cout << "\n\t\t";
                            SetConsoleTextAttribute(p, 8 | FOREGROUND_INTENSITY);
                            cout << " Расстановка в ручную ";
                            cout << "\n\t\t";
                            SetConsoleTextAttribute(p, 6 | FOREGROUND_INTENSITY);
                            cout << " Готово" << endl;


                        }

                        else if (act == 2)
                        {
                            if (provv == true)
                            {
                                gotovo = true;
                            }

                            else
                            {
                                gotovo = false;
                            }
                        }
                        break;

                    }

                    if (act < 0) act = 0;
                    if (act > num - 1) act = num - 1;
                }


            }

        }

        if (act < 0) act = 0;
        if (act > num - 1) act = num - 1;
    }


    //cin >> go;
    system("cls");

    int x = 0, y = 0;
    int dir = 0;
    int size = 4;


    bool turn = 1; // переменая для поочерёдного хода 

    bool winHum1 = 0;
    bool winHum2 = 0;



    while (winHum1 == false && winHum2 == false) // цикл переключения кода
    {
        int result = 0;

        do // цикл очередной стрильбы
        {
            cout << "_______Поле Человека1_________" << endl;
            map_show(map, mask, 1);

            cout << endl;

            cout << "_______Поле Человека2_________" << endl;
            map_show(map2, mask2, 1);


            if (turn == 1)
            {
                cout << "\tPlayer #1" << endl;
                cout << "Введите кординаты цели!" << endl;
                cout << "Введите х: ";
                cin >> x;
                cout << "Введите y: ";
                cin >> y;
                // x = rand() % n;
                // y = rand() % n;

                result = shot(map2, x, y, ships2, mask2);

                if (result == 1)
                {
                    system("cls");//////
                    cout << "Ранен" << endl;
                }
                else if (result == 2)
                {
                    bool died = 1;
                    for (int i = 1; i <= num_ships; i++)
                    {
                        if (ships2[i] != 0)
                        {
                            died = 0;
                            break;
                        }
                    }

                    if (died == 1)
                    {
                        winHum1 = 1;
                        break;
                    }
                    system("cls");//////
                    cout << "Убит!" << endl;
                }
                else
                {
                    system("cls");///////
                    cout << "Промах" << endl;
                }
            }


            else
            {
                cout << "\tPlayer #2" << endl;
                cout << "Введите кординаты цели!" << endl;
                cout << "Введите х: ";
                cin >> x;
                cout << "Введите y: ";
                cin >> y;

                // x = rand() % n;
                // y = rand() % n;

                result = shot(map, x, y, ships1, mask);

                if (result == 1)
                {
                    system("cls");//////
                    cout << "Ранен" << endl;
                }
                else if (result == 2)
                {
                    bool died = 1;
                    for (int i = 1; i <= num_ships; i++)
                    {
                        if (ships1[i] != 0)
                        {
                            died = 0;
                            break;
                        }
                    }

                    if (died == 1)
                    {
                        winHum2 = 1;
                        break;
                    }
                    system("cls");////////
                    cout << "Убит!" << endl;
                }
                else
                {
                    system("cls");//////
                    cout << "Промах" << endl;
                }


            }




        } while (result != 0);

        turn = !turn;
    }

    if (winHum1 == true)
    {
        cout << "Вы победили!" << endl;
    }

    else if (winHum2 == true)
    {
        cout << "Вы проиграли!" << endl;
    }


}




void game() // bot - human
{
    //(srand(time(0)));

    HANDLE p = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(p, 6 | FOREGROUND_INTENSITY);

    const int n = 10;

    // поля для человека и бота
    int map[n][n] = { 0 };
    int map2[n][n] = { 0 };

    // маска для туманой игры
    int mask[n][n] = { 0 };
    int mask2[n][n] = { 0 };

    // добавления в ручную кораблей
    int ships1[num_ships + 2] = { 0, 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
    int ships2[num_ships + 2] = { 0, 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };



    //случайная растановка кораблей для человека
    /*
    for (int i = 0; i <= num_ships; i++)
    {
        rand_ships(map, ships1[i], i);
    }
    */

    int x = 0, y = 0;
    int dir = 0;
    int size = 4;


    // случайная растановка кораблей для бота
    for (int i = 0; i <= num_ships; i++)
    {
        rand_ships(map2, ships2[i], i);
    }

    // rand_ships(map, 4, 1); // создание 4-х палубных в количестве 1-го корабля
    // rand_ships(map, 3, 2); // создание 3-х палубных в количесте 2-х кораблей
    // rand_ships(map, 2, 3); // создание 2-х палубных в количестве 3-х кораблей
    // rand_ships(map, 1, 4); // создание 1-х палубных в количестве 3-х кораблей


    // для второго поля
    // rand_ships(map2, 4, 1); // создание 4-х палубных в количестве 1-го корабля
    // rand_ships(map2, 3, 2); // создание 3-х палубных в количесте 2-х кораблей
    // rand_ships(map2, 2, 3); // создание 2-х палубных в количестве 3-х кораблей
    // rand_ships(map2, 1, 3); // создание 1-х палубных в количестве 3-х кораблей


    //int x=0, y=0;
    // int dir = 0;
    // int size = 4;
    // int ch;








    cout << "_______Поле Человека1_________" << endl;
    map_show(map, mask, 0);

    cout << endl;

    cout << "_______Поле Человека2_________" << endl;
    map_show(map2, mask2, 1);

    bool prov = false;

    const int nume = 3;

    int act = 0;

    int che = 0;

    bool gotovo = false;

    cout << "\n\n\t\t";
    cout << " Случайная расстановка ";
    cout << "\n\t\t";
    SetConsoleTextAttribute(p, 8 | FOREGROUND_INTENSITY);
    cout << " Расстановка в ручную ";
    cout << "\n\t\t";
    SetConsoleTextAttribute(p, 6 | FOREGROUND_INTENSITY);
    cout << " Готово" << endl;

    while (!gotovo)
    {
        gotoxy(-6, act + 25);

        che = _getch();
        if (che == 224)
            che = _getch();

        switch (che)
        {
        case 27: gotovo = true; break;   //27
        case 119: act--; break; // w вверх //119
        case 115: act++; break; // s вниз  //115
        case 13:

            if (act == 0)
            {

                system("cls");
                // srand(time(0));

                for (int i = 0; i < n; i++)
                {
                    for (int j = 0; j < n; j++)
                    {

                        if (map[j][i] == 0)
                        {
                            map[j][i] = 0;
                        }
                        else if (map[j][i] == -1)
                        {
                            map[j][i] = 0;
                        }
                        else if (map[j][i] == -2)
                        {
                            map[j][i] = 0;
                        }
                        else
                        {
                            map[j][i] = 0;
                        }
                    }
                }


                for (int i = 0; i <= num_ships; i++)
                {
                    rand_ships(map, ships1[i], i);
                }

                prov = true;

                cout << "_______Поле Человека1_________" << endl;
                map_show(map, mask, 0);

                cout << endl;

                cout << "_______Поле Человека2_________" << endl;
                map_show(map2, mask2, 1);


                cout << "\n\n\t\t";
                cout << " Случайная расстановка ";
                cout << "\n\t\t";
                SetConsoleTextAttribute(p, 8 | FOREGROUND_INTENSITY);
                cout << " Расстановка в ручную ";
                cout << "\n\t\t";
                SetConsoleTextAttribute(p, 6 | FOREGROUND_INTENSITY);
                cout << " Готово" << endl;

            }

            else if (act == 2)
            {
                if (prov == true)
                {
                    gotovo = true;
                }

                else
                {
                    gotovo = false;
                }
            }
            break;
        }

        if (act < 0) act = 0;
        if (act > nume - 1) act = nume - 1;

    }



    system("cls");




    //********************* - ИИ бота
    int hit_x = 0;
    int hit_y = 0;

    int mode = 0;

    int xBot = 0;
    int yBot = 0;

    //int dir = 0;

    vector <int> dirs;

    dirs.push_back(3);
    dirs.push_back(2);
    dirs.push_back(1);
    dirs.push_back(0);
    //*********************

    bool turn = 1; // переменая для поочерёдного хода 

    bool winHum = 0;
    bool winBot = 0;



    while (winHum == false && winBot == false) // цикл переключения кода
    {
        int result = 0;

        do // цикл очередной стрильбы
        {
            cout << "_______Поле Человека_________" << endl;
            map_show(map, mask, 0);

            cout << endl;

            cout << "_______Поле Компютера_________" << endl;
            map_show(map2, mask2, 0);


            if (turn == 1)
            {
                cout << "Введите кординаты цели!" << endl;
                cout << "Введите х: ";
                cin >> x;
                cout << "Введите y: ";
                cin >> y;

                result = shot(map2, x, y, ships2, mask2);

                if (result == 1)
                {
                    system("cls");
                    cout << "Ранен" << endl;
                }
                else if (result == 2)
                {
                    bool died = 1;
                    for (int i = 1; i <= num_ships; i++)
                    {
                        if (ships2[i] != 0)
                        {
                            died = 0;
                            break;
                        }
                    }

                    if (died == 1)
                    {
                        winHum = 1;
                        break;
                    }

                    system("cls");
                    cout << "Убит!" << endl;
                }
                else
                {
                    system("cls");
                    cout << "Промах" << endl;
                }
            }


            else
            {

                cout << "Ход компютера: " << endl;
                Sleep(5000);
                system("cls");
                // интелектуальная игра бота
                if (mode == 0)
                {
                    do // цикл повторного хода бота
                    {
                        xBot = rand() % n;
                        yBot = rand() % n;

                        result = shot(map, xBot, yBot, ships1, mask); // выстрел 
                    } while (result);

                    if (result == 1)
                    {
                        mode = 1;

                        hit_x = xBot;
                        hit_y = yBot;

                        if (!dirs.empty()) // если вектор не пуст
                        {
                            dir = dirs[dirs.size() - 1]; // извлечение направление
                            dirs.pop_back(); // удаление элемента
                        }

                        system("cls");
                        cout << "Ранен" << endl;
                    }
                    else if (result == 2)
                    {
                        bool died = 1;
                        for (int i = 1; i <= num_ships; i++)
                        {
                            if (ships1[i] != 0)
                            {
                                died = 0;
                                break;
                            }
                        }

                        if (died == 1)
                        {
                            winBot = 1;
                            break;
                        }
                        system("cls");
                        cout << "Убит" << endl;
                    }
                    else
                    {
                        system("cls");
                        cout << "Промах" << endl;
                    }
                }
                else if (mode == 1) // режим первого попадания (поиск)
                {
                    bool change = 0;

                    if (dir == 0) // влево
                    {
                        if (xBot > 0)
                            xBot--;
                        else
                        {
                            change = 1;
                        }
                    }
                    else if (dir == 1) // впрво
                    {
                        if (xBot < n - 1)
                            xBot++;
                        else
                        {
                            change = 1;
                        }
                    }
                    else if (dir == 2) // вверх
                    {
                        if (yBot > 0)
                            yBot--;
                        else
                        {
                            change = 1;
                        }
                    }
                    else if (dir == 3) // вниз
                    {
                        if (yBot < n - 1)
                            yBot++;
                        else
                        {
                            change = 1;
                        }
                    }
                    if (change == 1)
                    {
                        if (!dirs.empty())
                        {
                            dir = dirs[dirs.size() - 1];
                            dirs.pop_back();
                        }

                        xBot = hit_x;
                        yBot = hit_y;

                        continue;
                    }
                    result = shot(map, xBot, yBot, ships1, mask);

                    if (result == 1)
                    {
                        system("cls");
                        cout << "Ранен" << endl;
                    }
                    else if (result == 2)
                    {
                        mode = 0;

                        dirs.clear();

                        dirs.push_back(3);
                        dirs.push_back(2);
                        dirs.push_back(1);
                        dirs.push_back(0);

                        bool died = 1;
                        for (int i = 1; i <= num_ships; i++)
                        {
                            if (ships1[i] != 0)
                            {
                                died = 0;
                                break;
                            }
                        }

                        if (died == 1)
                        {
                            winBot = 1;
                            break;
                        }

                        cout << "Убит" << endl;
                    }
                    else
                    {
                        if (!dirs.empty())
                        {
                            dir = dirs[dirs.size() - 1];
                            dirs.pop_back();
                        }

                        xBot = hit_x;
                        yBot = hit_y;

                        cout << "Промах" << endl;
                    }

                }

            }

            Sleep(1000);

        } while (result != 0);

        turn = !turn;
    }

    if (winHum == true)
    {
        cout << "Вы победили!" << endl;
    }

    else if (winBot == true)
    {
        cout << "Вы проиграли!" << endl;
    }
}


void Bot_Bot()
{
    //(srand(time(0)));

    HANDLE p = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(p, 5 | FOREGROUND_INTENSITY);

    const int n = 10;

    // поля для человека и бота
    int map[n][n] = { 0 };
    int map2[n][n] = { 0 };

    // маска для туманой игры
    int mask[n][n] = { 0 };
    int mask2[n][n] = { 0 };

    // добавления в ручную кораблей
    int ships1[num_ships + 1] = { 0, 4, 3, 3, 2, 2, 2, 1, 1, 1 };
    int ships2[num_ships + 1] = { 0, 4, 3, 3, 2, 2, 2, 1, 1, 1 };

    //случайная растановка кораблей для человека
    for (int i = 0; i <= num_ships; i++)
    {
        rand_ships(map, ships1[i], i);
    }

    int x = 0, y = 0;
    int dir = 0;
    int size = 4;

    int x2 = 0, y2 = 0;
    int dir2 = 0;
    int size2 = 4;


    // случайная растановка кораблей для бота
    for (int i = 0; i <= num_ships; i++)
    {
        rand_ships(map2, ships2[i], i);
    }

    // rand_ships(map, 4, 1); // создание 4-х палубных в количестве 1-го корабля
    // rand_ships(map, 3, 2); // создание 3-х палубных в количесте 2-х кораблей
    // rand_ships(map, 2, 3); // создание 2-х палубных в количестве 3-х кораблей
    // rand_ships(map, 1, 4); // создание 1-х палубных в количестве 3-х кораблей


    // для второго поля
    // rand_ships(map2, 4, 1); // создание 4-х палубных в количестве 1-го корабля
    // rand_ships(map2, 3, 2); // создание 3-х палубных в количесте 2-х кораблей
    // rand_ships(map2, 2, 3); // создание 2-х палубных в количестве 3-х кораблей
    // rand_ships(map2, 1, 3); // создание 1-х палубных в количестве 3-х кораблей


    //int x=0, y=0;
    // int dir = 0;
    // int size = 4;
    // int ch;


    //********************* - ИИ бота
    int hit_x = 0;
    int hit_y = 0;

    int mode = 0;

    int xBot = 0;
    int yBot = 0;

    //int dir = 0;

    vector <int> dirs;

    dirs.push_back(3);
    dirs.push_back(2);
    dirs.push_back(1);
    dirs.push_back(0);
    //*********************


    //********************* - ИИ бота 2
    int hit_x2 = 0;
    int hit_y2 = 0;

    int mode2 = 0;

    int xBot2 = 0;
    int yBot2 = 0;

    //int dir = 0;

    vector <int> dirs2;

    dirs2.push_back(3);
    dirs2.push_back(2);
    dirs2.push_back(1);
    dirs2.push_back(0);
    //*********************

    bool turn = 1; // переменая для поочерёдного хода 

    bool winHum = 0;
    bool winBot = 0;

    while (winHum == false && winBot == false) // цикл переключения кода
    {
        int result = 0;
        //int result2 = 0;

        do // цикл очередной стрильбы
        {
            cout << "_______Поле Компютера1_________" << endl;
            map_show(map2, mask2, 0);

            cout << endl;

            cout << "_______Поле Компютера2_________" << endl;
            map_show(map, mask, 0);



            if (turn == 1)
            {

                cout << "Ход компютера1: " << endl;
                Sleep(5000);

                // интелектуальная игра бота
                if (mode2 == 0)
                {
                    do // цикл повторного хода бота
                    {

                        xBot2 = rand() % n;
                        yBot2 = rand() % n;

                        result = shot(map, xBot2, yBot2, ships1, mask); // выстрел 
                    } while (result);

                    if (result == 1)
                    {
                        mode2 = 1;

                        hit_x2 = xBot2;
                        hit_y2 = yBot2;

                        if (!dirs2.empty())
                        {
                            dir2 = dirs2[dirs2.size() - 1];
                            dirs2.pop_back();
                        }
                        system("cls");
                        cout << "Ранен" << endl;
                    }
                    else if (result == 2)
                    {
                        bool died2 = 1;
                        for (int i = 1; i <= num_ships; i++)
                        {
                            if (ships2[i] != 0)
                            {
                                died2 = 0;
                                break;
                            }
                        }

                        if (died2 == 1)
                        {
                            winBot = 1;
                            break;
                        }
                        system("cls");
                        cout << "Убит" << endl;
                    }
                    else
                    {
                        system("cls");
                        cout << "Промах" << endl;
                    }
                }
                else if (mode2 == 1)
                {
                    bool change2 = 0;

                    if (dir2 == 0) // влево
                    {
                        if (xBot2 > 0)
                            xBot2--;
                        else
                        {
                            change2 = 1;
                        }
                    }
                    else if (dir2 == 1) // впрво
                    {
                        if (xBot2 < n - 1)
                            xBot2++;
                        else
                        {
                            change2 = 1;
                        }
                    }
                    else if (dir2 == 2) // вверх
                    {
                        if (yBot2 > 0)
                            yBot2--;
                        else
                        {
                            change2 = 1;
                        }
                    }
                    else if (dir2 == 3) // вниз
                    {
                        if (yBot2 < n - 1)
                            yBot2++;
                        else
                        {
                            change2 = 1;
                        }
                    }
                    if (change2 == 1)
                    {
                        if (!dirs2.empty())
                        {
                            dir2 = dirs2[dirs2.size() - 1];
                            dirs2.pop_back();
                        }

                        xBot2 = hit_x2;
                        yBot2 = hit_y2;

                        continue;
                    }
                    result = shot(map, xBot2, yBot2, ships1, mask);

                    if (result == 1)
                    {
                        system("cls");
                        cout << "Ранен" << endl;
                    }
                    else if (result == 2)
                    {
                        mode2 = 0;

                        dirs2.clear();

                        dirs2.push_back(3);
                        dirs2.push_back(2);
                        dirs2.push_back(1);
                        dirs2.push_back(0);

                        bool died2 = 1;
                        for (int i = 1; i <= num_ships; i++)
                        {
                            if (ships2[i] != 0)
                            {
                                died2 = 0;
                                break;
                            }
                        }

                        if (died2 == 1)
                        {
                            winBot = 1;
                            break;
                        }
                        system("cls");
                        cout << "Убит" << endl;
                    }
                    else
                    {
                        if (!dirs2.empty())
                        {
                            dir2 = dirs2[dirs2.size() - 1];
                            dirs2.pop_back();
                        }

                        xBot2 = hit_x2;
                        yBot2 = hit_y2;
                        system("cls");
                        cout << "Промах" << endl;
                    }

                }

            }



            else
            {
                cout << "Ход компютера2: " << endl;
                Sleep(5000);

                // интелектуальная игра бота
                if (mode == 0)
                {
                    do // цикл повторного хода бота
                    {

                        xBot = rand() % n;
                        yBot = rand() % n;

                        result = shot(map2, xBot, yBot, ships2, mask2); // выстрел 
                    } while (result);

                    if (result == 1)
                    {
                        mode = 1;

                        hit_x = xBot;
                        hit_y = yBot;

                        if (!dirs.empty())
                        {
                            dir = dirs[dirs.size() - 1];
                            dirs.pop_back();
                        }
                        system("cls");
                        cout << "Ранен" << endl;
                    }
                    else if (result == 2)
                    {
                        bool died = 1;
                        for (int i = 1; i <= num_ships; i++)
                        {
                            if (ships1[i] != 0)
                            {
                                died = 0;
                                break;
                            }
                        }

                        if (died == 1)
                        {
                            winBot = 1;
                            break;
                        }
                        system("cls");
                        cout << "Убит" << endl;
                    }
                    else
                    {
                        system("cls");
                        cout << "Промах" << endl;
                    }
                }
                else if (mode == 1)
                {
                    bool change = 0;

                    if (dir == 0) // влево
                    {
                        if (xBot > 0)
                            xBot--;
                        else
                        {
                            change = 1;
                        }
                    }
                    else if (dir == 1) // впрво
                    {
                        if (xBot < n - 1)
                            xBot++;
                        else
                        {
                            change = 1;
                        }
                    }
                    else if (dir == 2) // вверх
                    {
                        if (yBot > 0)
                            yBot--;
                        else
                        {
                            change = 1;
                        }
                    }
                    else if (dir == 3) // вниз
                    {
                        if (yBot < n - 1)
                            yBot++;
                        else
                        {
                            change = 1;
                        }
                    }
                    if (change == 1)
                    {
                        if (!dirs.empty())
                        {
                            dir = dirs[dirs.size() - 1];
                            dirs.pop_back();
                        }

                        xBot = hit_x;
                        yBot = hit_y;

                        continue;
                    }
                    result = shot(map2, xBot, yBot, ships2, mask2);

                    if (result == 1)
                    {
                        system("cls");
                        cout << "Ранен" << endl;
                    }
                    else if (result == 2)
                    {
                        mode = 0;

                        dirs.clear();

                        dirs.push_back(3);
                        dirs.push_back(2);
                        dirs.push_back(1);
                        dirs.push_back(0);

                        bool died = 1;
                        for (int i = 1; i <= num_ships; i++)
                        {
                            if (ships1[i] != 0)
                            {
                                died = 0;
                                break;
                            }
                        }

                        if (died == 1)
                        {
                            winBot = 1;
                            break;
                        }
                        system("cls");
                        cout << "Убит" << endl;
                    }
                    else
                    {
                        if (!dirs.empty())
                        {
                            dir = dirs[dirs.size() - 1];
                            dirs.pop_back();
                        }

                        xBot = hit_x;
                        yBot = hit_y;

                        system("cls");
                        cout << "Промах" << endl;
                    }

                }

            }

            Sleep(100);

        } while (result != 0);

        turn = !turn;
    }

    if (winHum == true)
    {
        cout << "Вы победили!" << endl;
    }

    else if (winBot == true)
    {
        cout << "Вы проиграли!" << endl;
    }
}








void loading()
{
    system("cls");

    HANDLE p = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(p, 8 | FOREGROUND_INTENSITY);


    cout << "\n\n\n\n\n\t\t\t";
    cout << "   loading" << endl;
    string h = "\t\t   ........................"; // ** Искусственное ожидание с помощью имитации загрузки визуализированное точками
    for (int i = 0; i < h.length(); i++)
    {
        Sleep(50);
        cout << h[i];
    }
    Sleep(1000);
    system("cls");
}


void menu()
{

    HANDLE hwnd = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hwnd, 3 | FOREGROUND_INTENSITY);

    system("cls");
    cout << "\n\n";
    cout << "\t\t\t  MENU" << endl;
    cout << endl;
    cout << "\t\t\tNew Game" << endl;
    cout << "\t\t\tRegulations" << endl;
    cout << "\t\t\tExit" << endl;
}

void Menu_2()
{

    HANDLE p = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(p, 2 | FOREGROUND_INTENSITY);

    const int num = 4;

    int act = 0;

    int che = 0;

    bool exit = false;

    system("cls");
    cout << "\n\n";
    cout << "\t\t\t  Game Mode" << endl;
    cout << endl;
    cout << "\t\t\tHuman - Bot" << endl;
    cout << "\t\t\tHuman - Human" << endl;
    cout << "\t\t\tBot - Bot" << endl;
    cout << "\t\t\tExit" << endl;
    //system("pause");
    while (!exit)
    {
        gotoxy(0, act + 2);

        che = _getch();
        if (che == 224)
            che = _getch();

        switch (che)
        {
        case 27: exit = true; break;   //27
        case 'w': act--; break; // w вверх //119
        case 115: act++; break; // s вниз  //115
        case 13:
            if (act == 0)
            {
                system("cls");
                loading();
                system("cls");
                game();
            }

            else if (act == 1)
            {
                system("cls");
                loading();
                system("cls");
                human_game();
            }

            else if (act == 2)
            {
                system("cls");
                loading();
                system("cls");
                Bot_Bot();
            }

            else if (act == 3)
            {
                exit = true;
            }
            break;

        }

        if (act < 0) act = 0;
        if (act > num - 1) act = num - 1;
    }
}













int main()
{
    (srand(time(0)));
    setlocale(LC_ALL, "Russian");

    HWND window_header = GetConsoleWindow();
    SetWindowPos(window_header, HWND_TOPMOST, 400, 150, 500, 600, NULL);

    system("title See battle");

    system("cls");

    const int num_menu = 3;

    int active = 0;

    int ch = 0;

    bool exit = false;




    while (!exit)
    {

        menu();

        gotoxy(0, active + 2);

        // получение кода нажатой калвиши
        ch = _getch();
        if (ch == 224)
            ch = _getch();

        switch (ch)
        {
        case 27: exit = true; break;
        case 'w': active--; break; // w вверх
        case 115: active++; break; // s вниз
        case 13:
            if (active == 0)
            {
                system("cls");
                Menu_2();
            }

            else if (active == 1)
            {
                // правила
            }

            else if (active == 2)
            {
                exit = true;
            }
            break;

        }

        if (active < 0) active = 0;
        if (active > num_menu - 1) active = num_menu - 1;
    }

    return 0;


}





