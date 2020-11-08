#ifndef GAME_LIFE_HPP
#define GAME_LIFE_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include "DemTime.hpp"

using namespace sf;
using namespace std;

enum class GameMode { Default, Fullscreen, Fullscreen_Fill }; // Режим отображения окна

class GameLife
{
private:
	
	/* --- Все, что касается игрового поля игры --- */
	bool*** maps; // Вся история матриц (Основной массив матриц (Трехмерный массив))

	unsigned short int x; // Кол-во рядов у матриц
	unsigned short int y; // Кол-во колонн у матриц
	float updateTime; // Время обновления матриц после одного хода

	unsigned int capacity_z; // Емкость для матриц
	unsigned int size_z; // Количество матриц
	
	/* --- Размеры ячеек --- */
	float sizeX; // Длина ячейки
	float sizeY; // Ширина ячейки
	/* --------------------- */

	/* -------------------------------------------- */

	/* --- Режимы игры --- */
	GameMode gameMode; // Режим экрана (640х640 (или) Полноэкранный (или) Полноэкранный с авто-заполнением)
	bool draw; // Режим рисования первой матрицы (true/false => вкл./выкл.)
	bool closedPrematurely = false; // Переменная хранит информацию о том, была-ли закончена игра преждевременно
	/* ------------------- */

	float timeWasted = 0; // Время, прошедшее до замыкания цикла или смерти всех клеток

	/* --- Текстуры ---*/
	/**
	* !!! ВАЖНОЕ УПОМИНАНИЕ ИЗ ОФИЦИАЛЬНОЙ ДОКУМЕНТАЦИИ SFML !!!
	* Цитирую: "Когда вы устанавливаете текстуру спрайта, все, что он (спрайт) делает внутри,
	* - это сохраняет указатель на экземпляр текстуры.
	* Следовательно, если текстура будет уничтожена или перемещена в другое место в памяти,
	* спрайт окажется с недопустимым указателем текстуры."
	* 
	* И мы увидим квадрат одного цвета вместо текстуры.
	* По этой причине текстуры также объявлены в самом экземпляре класса, а не в одной из его функций.
	* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	**/
	sf::Texture cellTexture;
	sf::Texture livingCellTexture;
	/* --------------- */

	/* --- Спрайты --- */
	sf::Sprite cellSprite; // Спрайт пустой клетки
	sf::Sprite livingCellSprite; // Спрайт живой клетки
	/* --------------- */

	/* Функции жизненного цикла игры */

	/**
		@brief Основной цикл игры, последовательно вызывает функции start() -> processing() -> end().
	*/
	void mainCycle();

	/**
		@brief Начало игры. Здесь загружаются ресурсы игры, создаётся окно.
			Также эта функция отвечает за вызов функции drawing().
		@return true - при успешной загрузке всех ресурсов игры и создании окна
				false - при отсутствии файлов необходимых для игры
	*/
	bool start(sf::RenderWindow& window);

	/**
		@brief Создание новых матриц на основе предыдущих матриц с применением правил самой игры.
	*/
	void processing(sf::RenderWindow& window);

	/**
		@brief Конец игры, подведение итогов, освобождение памяти.
	*/
	void end(sf::RenderWindow& window);
	
	/**
		@brief Позволяет пользователю самому выделить с использованием ЛКМ живые клетки на первой матрице.
	*/
	void drawing(sf::RenderWindow& window);

	/* Вспомогательные функции */

	/**
		@brief Добавление новой матрицы в конец основного массива матриц.
	*/
	void pushBack(bool**& matrix);

	/**
		@brief Заполнение матрицы случайным значением - 0 или 1 (в контексте булевой переменной - false или true).
	*/
	void randomFill(bool** matrix, unsigned short int x, unsigned short int y);

	/**
		@brief Освобождение памяти трехмерного массива (указателя на указатели указывающих на другие указатели)
	*/
	void matrixFree(bool*** matrix, unsigned int capacity); // Удаление матрицы
public:
	/* Конструкторы */
	GameLife(float updateTime = 0.05f, bool draw = false); // По совместительству является DEFAULT-ным конструктором. (GameLife())
	GameLife(unsigned short int x, unsigned short int y, float updateTime, GameMode gameMode = GameMode::Default, bool draw = false);

	/* Цвета для ячеек */
	const sf::Color LIVING_CELL_COLOR = sf::Color(0, 255, 0); // Цвет живой ячейки
	const sf::Color IMMORTAL_CELL_COLOR = sf::Color(0, 255, 255); // Цвет ячейки обревшей бессмертие или
                                                                  // постоянное возрождение спустя ровно N итераций.
	const sf::Color DRAWING_CELL_COLOR = sf::Color(255, 255, 0); // Цвет живой ячейки в режиме рисования

	/* Пути до ресурсов игры */
	const string CELL_TEXTURE = "img/cell.png"; // Путь до изображения пустой ячейки
	const string LIVING_CELL_TEXTURE = "img/livecell.png"; // Путь до изображения живой ячейки
};

#endif // GAME_LIFE_HPP