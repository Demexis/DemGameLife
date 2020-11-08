// Игра "Жизнь" - клеточный автомат Джона Конуэя.

#include "GameLife.hpp"


GameLife::GameLife(float updateTime, bool draw): // DEFAULT CONSTRUCTOR
    updateTime(updateTime), gameMode(GameMode::Fullscreen_Fill), draw(draw), capacity_z(0), size_z(0)
{
    maps = new bool** [capacity_z]; // Выделение начальной памяти с целью избежать дальнейшей ошибки при вызове delete[] в pushBack()
    mainCycle(); // Вызов основного цикла игры
}

GameLife::GameLife(unsigned short int x, unsigned short int y, float updateTime, GameMode gameMode, bool draw):
    x(x), y(y), updateTime(updateTime), gameMode(gameMode), draw(draw), capacity_z(0), size_z(0)
{
    maps = new bool** [capacity_z]; // Выделение начальной памяти с целью избежать дальнейшей ошибки при вызове delete[] в pushBack()
    mainCycle(); // Вызов основного цикла игры
}

/**
    @brief Основной цикл игры, последовательно вызывает функции start() -> processing() -> end().
*/
void GameLife::mainCycle() // Основной цикл игры
{
    sf::RenderWindow window; // Объявление объекта класса "RenderWindow" для его дальнейшей инициализации.

    bool startSuccesful; // Переменная для обозначения успешной загрузки тех ресурсов,
                        // в которых в дальнейшем будут нуждаться функции processing() и end().

    startSuccesful = start(window); // Загрузка ресурсов и создание окна (инициализация)

    if (startSuccesful)
    {
        processing(window); // Основной цикл логики игры

        end(window); // Конец игры, подведение итогов, освобождение памяти
    }
    else return;
}

/**
    @brief Начало игры. Здесь загружаются ресурсы игры, создаётся окно.
        Также эта функция отвечает за вызов функции drawing().
    @return true - при успешной загрузке всех ресурсов игры и создании окна
            false - при отсутствии файлов необходимых для игры
*/
bool GameLife::start(sf::RenderWindow& window)
{
    /* --- Загрузка ресурсов --- */
    bool readyToContinue = true; // Пройдется со всеми необходимыми ресурсами игры
                                 // и если хотя бы одного файла нет - присвоит себе значение 'false'
                                 // и вернется функцией.

    if (!cellTexture.loadFromFile(CELL_TEXTURE)) // Пробуем загрузить первую текстуру пустой ячейки матрицы
    {
        cout << "Невозможно найти файл по адресу \"" << CELL_TEXTURE << "\"";
        readyToContinue = false;
    }

    if (!livingCellTexture.loadFromFile(LIVING_CELL_TEXTURE)) // Пробуем загрузить вторую текстуру живой ячейки матрицы
    {
        cout << "Невозможно найти файл по адресу \"" << LIVING_CELL_TEXTURE << "\"";
        readyToContinue = false;
    }

    if (!readyToContinue) return false;
    /* ------------------------- */

    /* --- Инициализация спрайтов --- */
    cellSprite.setTexture(cellTexture); // Спрайт с текстурой пустой клетки
    livingCellSprite.setTexture(livingCellTexture); // Спрайт с текстурой живой клетки
    /* ------------------------------ */

    /* --- Создание окна --- */
    // При полноэкранном режиме необходимо узнать разрешение монитора
    if (gameMode == GameMode::Fullscreen || gameMode == GameMode::Fullscreen_Fill)
    {
        unsigned short int screenSizeX = sf::VideoMode::getDesktopMode().width; // Ширина разрешения экрана
        unsigned short int screenSizeY = sf::VideoMode::getDesktopMode().height; // Длина разрешения экрана
        window.create(sf::VideoMode(screenSizeX, screenSizeY), "Game \"Life\"", sf::Style::Fullscreen);

        // При полноэкранном режиме с авто-заполнением экран заполняется ячейками с размером 20х20
        if (gameMode == GameMode::Fullscreen_Fill)
        {
            x = screenSizeX / 20;
            y = screenSizeY / 20;
        }
    }
    // В противном случае берём размер 640х640
    else
    {
        window.create(sf::VideoMode(640, 640), "Game \"Life\"");
    }

    std::cout << " --- ИНИЦИАЛИЗАЦИЯ ИГРЫ --- " << std::endl;
    std::cout << "  Размер матриц: [" << x << ", " << y << "]" << std::endl; // Сообщаем в консоль размер матриц
    /* --------------------- */

    /* --- Рассчитываем размер ячеек --- */
    sizeX = window.getSize().x / (float)x;
    sizeY = window.getSize().y / (float)y;
    /* --------------------------------- */

    std::cout << "  Размер ячеек: [" << sizeX << ", " << sizeY << "]" << std::endl; // Сообщаем в консоль размер ячеек
    std::cout << " -------------------------- " << std::endl;

    // Подстраиваем размеры спрайтов изменяя их масштаб
    cellSprite.setScale(sizeX / cellSprite.getTexture()->getSize().x, sizeY / cellSprite.getTexture()->getSize().y);
    livingCellSprite.setScale(sizeX / livingCellSprite.getTexture()->getSize().x, sizeY / livingCellSprite.getTexture()->getSize().y);

    /* --- Создаём начальную матрицу --- */
    // Выделяем память для указателей на первую (пустую) матрицу
    bool** firstMatrix = new bool* [x];
    for (int i = 0; i < x; i++)
    {
        firstMatrix[i] = new bool[y] {};
    }

    pushBack(firstMatrix); // Загоняем её в основной массив матриц
                            // Таким образом мы расширяем изначальную емкость массива матриц (из 0 ед. в 1 ед.) 

    // Освобождаем память указателей на первую (пустую) матрицу
    for (int i = 0; i < x; i++)
    {
        delete[] firstMatrix[i];
    }
    delete[] firstMatrix;
    
    if (draw)
    {
        drawing(window); // Вызов функции для выделения живых клеток в начале самим пользователем
    }
    else
    {
        srand(static_cast<unsigned int>(time(0))); // Садим генератор случайных чисел
        GameLife::randomFill(maps[size_z - 1], x, y); // Заполняем первую матрицу (уже в массиве матриц) случайными числами
    }

    return readyToContinue; // Если переменная осталась нетронутой - 
                            // возвращает своё инициализированное значение в самом начале - 'true'
}

/**
    @brief Создание новых матриц на основе предыдущих матриц с применением правил самой игры.
*/
void GameLife::processing(sf::RenderWindow& window)
{
    bool gameOver = false; // Переменная, которая будет обозначать конец игры

    /* --- Основной цикл игры --- */
    while (!gameOver && window.isOpen())
    {
        // Проверить все события окна, которые были запущены с момента последней итерации цикла
        sf::Event event;
        while (window.pollEvent(event))
        {
            // При нажатии на кнопку X в правом-верхнем углу.
            if (event.type == sf::Event::Closed)
            {
                closedPrematurely = true;
                window.close();
            }

            // Обработка события - нажатие кнопки на клавиатуре.
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Enter) // Enter
                {
                    closedPrematurely = true;
                    window.close();
                }

                if (event.key.code == sf::Keyboard::Escape) // Esc
                {
                    closedPrematurely = true;
                    window.close();
                }
            }
        }

        window.clear(sf::Color::Black); // Очистить окно черным цветом

        timeWasted += DemTime::WaitForTime(updateTime); // Задержка перед каждой итерации игры

        /* --- Отрисовка поля --- */
        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++)
            {
                cellSprite.setPosition(sizeX * i, sizeY * j);
                window.draw(cellSprite);

                if (maps[size_z - 1][i][j])
                {
                    livingCellSprite.setPosition(sizeX * i, sizeY * j);
                    livingCellSprite.setColor(LIVING_CELL_COLOR);
                    window.draw(livingCellSprite);
                }
            }
        }
        window.display(); // Закончить текущий кадр (отрисовать всё)
        /* ---------------------- */

        // Выделение памяти под новую матрицу
        bool** mapUpdate = new bool* [x];
        for (int i = 0; i < x; i++)
        {
            mapUpdate[i] = new bool[y] {};
        }

        // Ход
        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++)
            {
                int nearAlive = 0; // Кол-во живых клеток вокруг проверяемой

                /* Подсчёт кол-ва живых клеток рядом с той которую мы сейчас просматриваем во время итерации */
                if (maps[size_z - 1][(x + x - (x - i) - 1) % x][j]) nearAlive++; // Левый
                if (maps[size_z - 1][(x + x - (x - i) + 1) % x][j]) nearAlive++; // Правый

                if (maps[size_z - 1][i][(y + y - (y - j) - 1) % y]) nearAlive++; // Верхний
                if (maps[size_z - 1][i][(y + y - (y - j) + 1) % y]) nearAlive++; // Нижний

                if (maps[size_z - 1][(x + x - (x - i) - 1) % x][(y + y - (y - j) - 1) % y]) nearAlive++; // Левый-Верхний
                if (maps[size_z - 1][(x + x - (x - i) - 1) % x][(y + y - (y - j) + 1) % y]) nearAlive++; // Левый-Нижний

                if (maps[size_z - 1][(x + x - (x - i) + 1) % x][(y + y - (y - j) - 1) % y]) nearAlive++; // Правый-Верхний
                if (maps[size_z - 1][(x + x - (x - i) + 1) % x][(y + y - (y - j) + 1) % y]) nearAlive++; // Правый-Нижний
                /* ----------------------------------------------------------------------------------------- */

                // Судьба ячейки записывается в новую матрицу
                mapUpdate[i][j] = ((!maps[size_z - 1][i][j]) && (nearAlive == 3)) || ((maps[size_z - 1][i][j]) && (nearAlive == 2 || nearAlive == 3));
            }
        }

        // Применить добавление значений новой итерации жизни
        pushBack(mapUpdate);

        // Освободить память
        for (int i = 0; i < x; i++)
        {
            delete[] mapUpdate[i];
        }
        delete[] mapUpdate;

        // Проверить, не закончена-ли игра
        for (int i = 0; i < (size_z - 1) && !gameOver; i++)
        {
            bool cyclicMatrix = true;

            for (int j = 0; j < x && cyclicMatrix; j++)
            {
                for (int k = 0; k < y && cyclicMatrix; k++)
                {
                    if (maps[size_z - 1][j][k] != maps[i][j][k])
                    {
                        cyclicMatrix = false;
                    }
                }
            }

            gameOver = cyclicMatrix;
        }
    }
}

/**
    @brief Конец игры, подведение итогов, освобождение памяти.
*/
void GameLife::end(sf::RenderWindow& window)
{
    /* --- Конец игры, подведение итогов, освобождение памяти --- */
    std::cout << " --- СТАТИСТИКА ИГРЫ --- " << std::endl;
    std::cout << "  Время игры: " << timeWasted << " секунд." << std::endl;
    std::cout << "  Кол-во созданных матриц: " << size_z << " ед." << std::endl;
    std::cout << "  Кол-во зарезервированных мест для матриц: " << capacity_z << " ед." << std::endl;
    std::cout << " ----------------------- " << std::endl << std::endl;

    if (closedPrematurely)
    {
        std::cout << "Игра была прервана пользователем." << std::endl << std::endl;
    }
    else
    {
        window.clear(sf::Color::Black); // Очистить окно черным цветом
        bool dead = true;
        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++)
            {
                cellSprite.setPosition(sizeX * i, sizeY * j);
                window.draw(cellSprite);

                if (maps[size_z - 1][i][j])
                {
                    dead = false;

                    // Отрисовка ячеек последней матрицы в массиве другим цветом
                    livingCellSprite.setPosition(sizeX * i, sizeY * j);
                    livingCellSprite.setColor(IMMORTAL_CELL_COLOR);
                    window.draw(livingCellSprite);
                }
            }
        }
        window.display(); // Закончить текущий кадр (отрисовать всё)

        if (dead) std::cout << "Игра окончена. Все клетки погибли." << std::endl;
        else
        {
            std::cout << "Игра окончена. Все клетки обрели вечную жизнь." << std::endl;
            std::cout << "Либо застряли в бесконечном цикле возрождения с одними и теми же позициями." << std::endl << std::endl;
        }
    }

    matrixFree(maps, capacity_z); // Конец игры, освобождение памяти из кучи
    /* ------------------------------------- */

    if(window.isOpen()) std::cout << "Нажмите Enter для закрытия окна" << std::endl;

    while (window.isOpen())
    {
        // Проверить все события окна, которые были запущены с момента последней итерации цикла
        sf::Event event;
        while (window.pollEvent(event))
        {
            // При нажатии на кнопку X в правом-верхнем углу.
            if (event.type == sf::Event::Closed)
                window.close();

            // Обработка события - нажатие кнопки на клавиатуре.
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Enter) // Enter
                    window.close();

                if (event.key.code == sf::Keyboard::Escape) // Esc
                    window.close();
            }
        }
    }
}

/**
    @brief Позволяет пользователю самому выделить с использованием ЛКМ живые клетки на первой матрице.
*/
void GameLife::drawing(sf::RenderWindow& window)
{
    bool enterPressed = false;

    while (!enterPressed && window.isOpen())
    {
        window.clear();
        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++)
            {
                cellSprite.setPosition(sizeX * i, sizeY * j);
                window.draw(cellSprite);

                if (maps[size_z - 1][i][j])
                {
                    livingCellSprite.setPosition(sizeX * i, sizeY * j);
                    livingCellSprite.setColor(DRAWING_CELL_COLOR);
                    window.draw(livingCellSprite);
                }
            }
        }
        window.display();

        // Проверить все события окна, которые были запущены с момента последней итерации цикла
        sf::Event event;
        while (window.pollEvent(event))
        {
            // При нажатии на кнопку X в правом-верхнем углу.
            if (event.type == sf::Event::Closed)
            {
                closedPrematurely = true;
                window.close();
            }

            // Обработка события - нажатие кнопки на клавиатуре.
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Enter) // Enter
                    enterPressed = true;

                if (event.key.code == sf::Keyboard::Escape) // Esc
                {
                    closedPrematurely = true;
                    window.close();
                }
            }

            // Обработка события - нажатие кнопки мыши.
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left) // ЛКМ
                {
                    // Проверка на нажатие вне области матрицы
                    if (!(event.mouseButton.x < 0 || event.mouseButton.x > window.getSize().x
                        || event.mouseButton.y < 0 || event.mouseButton.y > window.getSize().y))
                    {
                        int iX = event.mouseButton.x / sizeX; // Получение индекса ряда матрицы относительно координаты мыши по оси X
                        int jY = event.mouseButton.y / sizeY; // Получение индекса ряда матрицы относительно координаты мыши по оси Y

                        maps[size_z - 1][iX][jY] = !maps[size_z - 1][iX][jY]; // Меняем значение ячейки последней матрицы
                                                                              // по индексам 'iX' и 'iY' на противоположное.
                                                                              // Таким образом пользователь всегда может 
                                                                              // обратно сделать ячейку мертвой.
                    }
                }
            }
        }
    }
}

/**
    @brief Добавление новой матрицы в конец основного массива матриц.
*/
void GameLife::pushBack(bool**& matrix)
{
    unsigned int oldCapacity = capacity_z;

    if (size_z == capacity_z)
    {
        // Удваиваем ёмкость
        capacity_z *= 2;

        if (capacity_z == 0)
        {
            capacity_z = 1;
        }

        /* --- Сохраняем элементы основного динамического массива прежде чем освободить память --- */
        bool*** save = new bool** [capacity_z];
        for (int i = 0; i < capacity_z; i++)
        {
            save[i] = new bool* [x];
            for (int j = 0; j < x; j++)
            {
                save[i][j] = new bool[y];
            }
        }

        for (int i = 0; i < size_z; i++)
        {
            for (int j = 0; j < x; j++)
            {
                for (int k = 0; k < y; k++)
                {
                    save[i][j][k] = maps[i][j][k];
                }
            }
        }
        /* --------------------------------------------------------------------------------------- */

        // Освобождаем память основного массива.
        matrixFree(maps, oldCapacity);

        // Выделяем вдвое большую память основному массиву.
        maps = new bool** [capacity_z];
        for (int i = 0; i < capacity_z; i++)
        {
            maps[i] = new bool* [x];
            for (int j = 0; j < x; j++)
            {
                maps[i][j] = new bool[y];
            }
        }

        // Восстанавливаем сохраненные элементы.
        for (int i = 0; i < size_z; i++)
        {
            for (int j = 0; j < x; j++)
            {
                for (int k = 0; k < y; k++)
                {
                    maps[i][j][k] = save[i][j][k];
                }
            }
        }

        // Не забываем освободить память которая помогла нам временно сохранить элементы в другом динамическом массиве.
        matrixFree(save, capacity_z);
    }

    // Увеличиваем размер основного динамического массива на одну единицу
    size_z++;

    // Добавляем новую матрицу
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            maps[size_z - 1][i][j] = matrix[i][j];
        }
    }
}

/**
    @brief Заполнение матрицы случайным значением - 0 или 1 (в контексте булевой переменной - false или true).
*/
void GameLife::randomFill(bool** matrix, unsigned short int x, unsigned short int y)
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            *(*(matrix + i) + j) = rand() % 2;
            // *(*(matrix + i) + j) <=> matrix[i][j]
        }
    }
}

/**
    @brief Освобождение памяти трехмерного массива (указателя на указатели указывающих на другие указатели)
*/
void GameLife::matrixFree(bool*** matrix, unsigned int capacity)
{
    for (int i = 0; i < capacity; i++)
    {
        for (int j = 0; j < x; j++) 
        {
            delete[] matrix[i][j];
        }
        delete[] matrix[i];
    }
    delete[] matrix;
}