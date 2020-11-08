// ���� "�����" - ��������� ������� ����� ������.

#include "GameLife.hpp"


GameLife::GameLife(float updateTime, bool draw): // DEFAULT CONSTRUCTOR
    updateTime(updateTime), gameMode(GameMode::Fullscreen_Fill), draw(draw), capacity_z(0), size_z(0)
{
    maps = new bool** [capacity_z]; // ��������� ��������� ������ � ����� �������� ���������� ������ ��� ������ delete[] � pushBack()
    mainCycle(); // ����� ��������� ����� ����
}

GameLife::GameLife(unsigned short int x, unsigned short int y, float updateTime, GameMode gameMode, bool draw):
    x(x), y(y), updateTime(updateTime), gameMode(gameMode), draw(draw), capacity_z(0), size_z(0)
{
    maps = new bool** [capacity_z]; // ��������� ��������� ������ � ����� �������� ���������� ������ ��� ������ delete[] � pushBack()
    mainCycle(); // ����� ��������� ����� ����
}

/**
    @brief �������� ���� ����, ��������������� �������� ������� start() -> processing() -> end().
*/
void GameLife::mainCycle() // �������� ���� ����
{
    sf::RenderWindow window; // ���������� ������� ������ "RenderWindow" ��� ��� ���������� �������������.

    bool startSuccesful; // ���������� ��� ����������� �������� �������� ��� ��������,
                        // � ������� � ���������� ����� ��������� ������� processing() � end().

    startSuccesful = start(window); // �������� �������� � �������� ���� (�������������)

    if (startSuccesful)
    {
        processing(window); // �������� ���� ������ ����

        end(window); // ����� ����, ���������� ������, ������������ ������
    }
    else return;
}

/**
    @brief ������ ����. ����� ����������� ������� ����, �������� ����.
        ����� ��� ������� �������� �� ����� ������� drawing().
    @return true - ��� �������� �������� ���� �������� ���� � �������� ����
            false - ��� ���������� ������ ����������� ��� ����
*/
bool GameLife::start(sf::RenderWindow& window)
{
    /* --- �������� �������� --- */
    bool readyToContinue = true; // ��������� �� ����� ������������ ��������� ����
                                 // � ���� ���� �� ������ ����� ��� - �������� ���� �������� 'false'
                                 // � �������� ��������.

    if (!cellTexture.loadFromFile(CELL_TEXTURE)) // ������� ��������� ������ �������� ������ ������ �������
    {
        cout << "���������� ����� ���� �� ������ \"" << CELL_TEXTURE << "\"";
        readyToContinue = false;
    }

    if (!livingCellTexture.loadFromFile(LIVING_CELL_TEXTURE)) // ������� ��������� ������ �������� ����� ������ �������
    {
        cout << "���������� ����� ���� �� ������ \"" << LIVING_CELL_TEXTURE << "\"";
        readyToContinue = false;
    }

    if (!readyToContinue) return false;
    /* ------------------------- */

    /* --- ������������� �������� --- */
    cellSprite.setTexture(cellTexture); // ������ � ��������� ������ ������
    livingCellSprite.setTexture(livingCellTexture); // ������ � ��������� ����� ������
    /* ------------------------------ */

    /* --- �������� ���� --- */
    // ��� ������������� ������ ���������� ������ ���������� ��������
    if (gameMode == GameMode::Fullscreen || gameMode == GameMode::Fullscreen_Fill)
    {
        unsigned short int screenSizeX = sf::VideoMode::getDesktopMode().width; // ������ ���������� ������
        unsigned short int screenSizeY = sf::VideoMode::getDesktopMode().height; // ����� ���������� ������
        window.create(sf::VideoMode(screenSizeX, screenSizeY), "Game \"Life\"", sf::Style::Fullscreen);

        // ��� ������������� ������ � ����-����������� ����� ����������� �������� � �������� 20�20
        if (gameMode == GameMode::Fullscreen_Fill)
        {
            x = screenSizeX / 20;
            y = screenSizeY / 20;
        }
    }
    // � ��������� ������ ���� ������ 640�640
    else
    {
        window.create(sf::VideoMode(640, 640), "Game \"Life\"");
    }

    std::cout << " --- ������������� ���� --- " << std::endl;
    std::cout << "  ������ ������: [" << x << ", " << y << "]" << std::endl; // �������� � ������� ������ ������
    /* --------------------- */

    /* --- ������������ ������ ����� --- */
    sizeX = window.getSize().x / (float)x;
    sizeY = window.getSize().y / (float)y;
    /* --------------------------------- */

    std::cout << "  ������ �����: [" << sizeX << ", " << sizeY << "]" << std::endl; // �������� � ������� ������ �����
    std::cout << " -------------------------- " << std::endl;

    // ������������ ������� �������� ������� �� �������
    cellSprite.setScale(sizeX / cellSprite.getTexture()->getSize().x, sizeY / cellSprite.getTexture()->getSize().y);
    livingCellSprite.setScale(sizeX / livingCellSprite.getTexture()->getSize().x, sizeY / livingCellSprite.getTexture()->getSize().y);

    /* --- ������ ��������� ������� --- */
    // �������� ������ ��� ���������� �� ������ (������) �������
    bool** firstMatrix = new bool* [x];
    for (int i = 0; i < x; i++)
    {
        firstMatrix[i] = new bool[y] {};
    }

    pushBack(firstMatrix); // �������� � � �������� ������ ������
                            // ����� ������� �� ��������� ����������� ������� ������� ������ (�� 0 ��. � 1 ��.) 

    // ����������� ������ ���������� �� ������ (������) �������
    for (int i = 0; i < x; i++)
    {
        delete[] firstMatrix[i];
    }
    delete[] firstMatrix;
    
    if (draw)
    {
        drawing(window); // ����� ������� ��� ��������� ����� ������ � ������ ����� �������������
    }
    else
    {
        srand(static_cast<unsigned int>(time(0))); // ����� ��������� ��������� �����
        GameLife::randomFill(maps[size_z - 1], x, y); // ��������� ������ ������� (��� � ������� ������) ���������� �������
    }

    return readyToContinue; // ���� ���������� �������� ���������� - 
                            // ���������� ��� ������������������ �������� � ����� ������ - 'true'
}

/**
    @brief �������� ����� ������ �� ������ ���������� ������ � ����������� ������ ����� ����.
*/
void GameLife::processing(sf::RenderWindow& window)
{
    bool gameOver = false; // ����������, ������� ����� ���������� ����� ����

    /* --- �������� ���� ���� --- */
    while (!gameOver && window.isOpen())
    {
        // ��������� ��� ������� ����, ������� ���� �������� � ������� ��������� �������� �����
        sf::Event event;
        while (window.pollEvent(event))
        {
            // ��� ������� �� ������ X � ������-������� ����.
            if (event.type == sf::Event::Closed)
            {
                closedPrematurely = true;
                window.close();
            }

            // ��������� ������� - ������� ������ �� ����������.
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

        window.clear(sf::Color::Black); // �������� ���� ������ ������

        timeWasted += DemTime::WaitForTime(updateTime); // �������� ����� ������ �������� ����

        /* --- ��������� ���� --- */
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
        window.display(); // ��������� ������� ���� (���������� ��)
        /* ---------------------- */

        // ��������� ������ ��� ����� �������
        bool** mapUpdate = new bool* [x];
        for (int i = 0; i < x; i++)
        {
            mapUpdate[i] = new bool[y] {};
        }

        // ���
        for (int i = 0; i < x; i++)
        {
            for (int j = 0; j < y; j++)
            {
                int nearAlive = 0; // ���-�� ����� ������ ������ �����������

                /* ������� ���-�� ����� ������ ����� � ��� ������� �� ������ ������������� �� ����� �������� */
                if (maps[size_z - 1][(x + x - (x - i) - 1) % x][j]) nearAlive++; // �����
                if (maps[size_z - 1][(x + x - (x - i) + 1) % x][j]) nearAlive++; // ������

                if (maps[size_z - 1][i][(y + y - (y - j) - 1) % y]) nearAlive++; // �������
                if (maps[size_z - 1][i][(y + y - (y - j) + 1) % y]) nearAlive++; // ������

                if (maps[size_z - 1][(x + x - (x - i) - 1) % x][(y + y - (y - j) - 1) % y]) nearAlive++; // �����-�������
                if (maps[size_z - 1][(x + x - (x - i) - 1) % x][(y + y - (y - j) + 1) % y]) nearAlive++; // �����-������

                if (maps[size_z - 1][(x + x - (x - i) + 1) % x][(y + y - (y - j) - 1) % y]) nearAlive++; // ������-�������
                if (maps[size_z - 1][(x + x - (x - i) + 1) % x][(y + y - (y - j) + 1) % y]) nearAlive++; // ������-������
                /* ----------------------------------------------------------------------------------------- */

                // ������ ������ ������������ � ����� �������
                mapUpdate[i][j] = ((!maps[size_z - 1][i][j]) && (nearAlive == 3)) || ((maps[size_z - 1][i][j]) && (nearAlive == 2 || nearAlive == 3));
            }
        }

        // ��������� ���������� �������� ����� �������� �����
        pushBack(mapUpdate);

        // ���������� ������
        for (int i = 0; i < x; i++)
        {
            delete[] mapUpdate[i];
        }
        delete[] mapUpdate;

        // ���������, �� ���������-�� ����
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
    @brief ����� ����, ���������� ������, ������������ ������.
*/
void GameLife::end(sf::RenderWindow& window)
{
    /* --- ����� ����, ���������� ������, ������������ ������ --- */
    std::cout << " --- ���������� ���� --- " << std::endl;
    std::cout << "  ����� ����: " << timeWasted << " ������." << std::endl;
    std::cout << "  ���-�� ��������� ������: " << size_z << " ��." << std::endl;
    std::cout << "  ���-�� ����������������� ���� ��� ������: " << capacity_z << " ��." << std::endl;
    std::cout << " ----------------------- " << std::endl << std::endl;

    if (closedPrematurely)
    {
        std::cout << "���� ���� �������� �������������." << std::endl << std::endl;
    }
    else
    {
        window.clear(sf::Color::Black); // �������� ���� ������ ������
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

                    // ��������� ����� ��������� ������� � ������� ������ ������
                    livingCellSprite.setPosition(sizeX * i, sizeY * j);
                    livingCellSprite.setColor(IMMORTAL_CELL_COLOR);
                    window.draw(livingCellSprite);
                }
            }
        }
        window.display(); // ��������� ������� ���� (���������� ��)

        if (dead) std::cout << "���� ��������. ��� ������ �������." << std::endl;
        else
        {
            std::cout << "���� ��������. ��� ������ ������ ������ �����." << std::endl;
            std::cout << "���� �������� � ����������� ����� ����������� � ������ � ���� �� ���������." << std::endl << std::endl;
        }
    }

    matrixFree(maps, capacity_z); // ����� ����, ������������ ������ �� ����
    /* ------------------------------------- */

    if(window.isOpen()) std::cout << "������� Enter ��� �������� ����" << std::endl;

    while (window.isOpen())
    {
        // ��������� ��� ������� ����, ������� ���� �������� � ������� ��������� �������� �����
        sf::Event event;
        while (window.pollEvent(event))
        {
            // ��� ������� �� ������ X � ������-������� ����.
            if (event.type == sf::Event::Closed)
                window.close();

            // ��������� ������� - ������� ������ �� ����������.
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
    @brief ��������� ������������ ������ �������� � �������������� ��� ����� ������ �� ������ �������.
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

        // ��������� ��� ������� ����, ������� ���� �������� � ������� ��������� �������� �����
        sf::Event event;
        while (window.pollEvent(event))
        {
            // ��� ������� �� ������ X � ������-������� ����.
            if (event.type == sf::Event::Closed)
            {
                closedPrematurely = true;
                window.close();
            }

            // ��������� ������� - ������� ������ �� ����������.
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

            // ��������� ������� - ������� ������ ����.
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left) // ���
                {
                    // �������� �� ������� ��� ������� �������
                    if (!(event.mouseButton.x < 0 || event.mouseButton.x > window.getSize().x
                        || event.mouseButton.y < 0 || event.mouseButton.y > window.getSize().y))
                    {
                        int iX = event.mouseButton.x / sizeX; // ��������� ������� ���� ������� ������������ ���������� ���� �� ��� X
                        int jY = event.mouseButton.y / sizeY; // ��������� ������� ���� ������� ������������ ���������� ���� �� ��� Y

                        maps[size_z - 1][iX][jY] = !maps[size_z - 1][iX][jY]; // ������ �������� ������ ��������� �������
                                                                              // �� �������� 'iX' � 'iY' �� ���������������.
                                                                              // ����� ������� ������������ ������ ����� 
                                                                              // ������� ������� ������ �������.
                    }
                }
            }
        }
    }
}

/**
    @brief ���������� ����� ������� � ����� ��������� ������� ������.
*/
void GameLife::pushBack(bool**& matrix)
{
    unsigned int oldCapacity = capacity_z;

    if (size_z == capacity_z)
    {
        // ��������� �������
        capacity_z *= 2;

        if (capacity_z == 0)
        {
            capacity_z = 1;
        }

        /* --- ��������� �������� ��������� ������������� ������� ������ ��� ���������� ������ --- */
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

        // ����������� ������ ��������� �������.
        matrixFree(maps, oldCapacity);

        // �������� ����� ������� ������ ��������� �������.
        maps = new bool** [capacity_z];
        for (int i = 0; i < capacity_z; i++)
        {
            maps[i] = new bool* [x];
            for (int j = 0; j < x; j++)
            {
                maps[i][j] = new bool[y];
            }
        }

        // ��������������� ����������� ��������.
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

        // �� �������� ���������� ������ ������� ������� ��� �������� ��������� �������� � ������ ������������ �������.
        matrixFree(save, capacity_z);
    }

    // ����������� ������ ��������� ������������� ������� �� ���� �������
    size_z++;

    // ��������� ����� �������
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            maps[size_z - 1][i][j] = matrix[i][j];
        }
    }
}

/**
    @brief ���������� ������� ��������� ��������� - 0 ��� 1 (� ��������� ������� ���������� - false ��� true).
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
    @brief ������������ ������ ����������� ������� (��������� �� ��������� ����������� �� ������ ���������)
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