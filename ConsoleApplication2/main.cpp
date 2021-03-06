﻿/**
    Игра "Жизнь"

    @brief Клеточный автомат Джона Конуэя, написанный на C++ с использованием SFML
    (Simple and Fast Multimedia Library).

    !!! ПРАВИЛА ИГРЫ (источник: https://ru.wikipedia.org/wiki/%D0%98%D0%B3%D1%80%D0%B0_%C2%AB%D0%96%D0%B8%D0%B7%D0%BD%D1%8C%C2%BB) !!!

    Место действия этой игры — размеченная на клетки замкнутая плоскость (в дальнейшем - матрица).
    Каждая клетка на этой матрице может находиться в двух состояниях: быть «живой» (заполненной) или быть «мёртвой» (пустой).
    Клетка имеет восемь соседей, окружающих её.
    
    Распределение живых клеток в начале игры называется первым поколением (в дальнейшем - первая матрица). 
    Каждая следующая матрица рассчитывается на основе предыдущей по таким правилам:
    * в пустой (мёртвой) клетке, рядом с которой ровно три живые клетки, зарождается жизнь;
    * если у живой клетки есть две или три живые соседки, то эта клетка продолжает жить;
      в противном случае, если соседей меньше двух или больше трёх, клетка умирает («от одиночества» или «от перенаселённости»)
    
    Игра прекращается, если:
    * на поле не останется ни одной «живой» клетки
    * конфигурация на очередном шаге в точности повторит себя же
      на одном из более ранних шагов (более ранних матриц)

    Эти простые правила приводят к огромному разнообразию форм, которые могут возникнуть в игре.

    Игрок не принимает прямого участия в игре, а лишь расставляет или генерирует начальную конфигурацию «живых» клеток,
    которые затем взаимодействуют согласно правилам уже без его участия (он является наблюдателем).

    !!!

    @author Девид Сорочинский (Devid Sorochinsky aka Demexis)

    @date 08.11.2020. (8 ноября 2020 г.)
*/

#include <iostream>
#include "GameLife.hpp"

using namespace sf;
using namespace std;

int main()
{
    setlocale(LC_ALL, "Rus"); // Локализация консоли для корректного отображения символов Юникода.

    GameLife game = GameLife(); // float updateTime = 0.05f, bool draw = false
    //GameLife game2 = GameLife(0.35f); // float updateTime = 0.35f, bool draw = false
    //GameLife game3 = GameLife(0.5f, true); // float updateTime = 0.5f, bool draw = true
    //GameLife game4 = GameLife(10, 10, 0.05f);
    //GameLife game5 = GameLife(15, 15, 0.25f, GameMode::Default);
    //GameLife game6 = GameLife(40, 40, 0.05f, GameMode::Default, true);

    cout << "Нажмите Enter для завершения" << endl;
    std::cin.get();

    return 0;
}