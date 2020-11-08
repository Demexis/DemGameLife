#ifndef GAME_LIFE_HPP
#define GAME_LIFE_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include "DemTime.hpp"

using namespace sf;
using namespace std;

enum class GameMode { Default, Fullscreen, Fullscreen_Fill }; // ����� ����������� ����

class GameLife
{
private:
	
	/* --- ���, ��� �������� �������� ���� ���� --- */
	bool*** maps; // ��� ������� ������ (�������� ������ ������ (���������� ������))

	unsigned short int x; // ���-�� ����� � ������
	unsigned short int y; // ���-�� ������ � ������
	float updateTime; // ����� ���������� ������ ����� ������ ����

	unsigned int capacity_z; // ������� ��� ������
	unsigned int size_z; // ���������� ������
	
	/* --- ������� ����� --- */
	float sizeX; // ����� ������
	float sizeY; // ������ ������
	/* --------------------- */

	/* -------------------------------------------- */

	/* --- ������ ���� --- */
	GameMode gameMode; // ����� ������ (640�640 (���) ������������� (���) ������������� � ����-�����������)
	bool draw; // ����� ��������� ������ ������� (true/false => ���./����.)
	bool closedPrematurely = false; // ���������� ������ ���������� � ���, ����-�� ��������� ���� ��������������
	/* ------------------- */

	float timeWasted = 0; // �����, ��������� �� ��������� ����� ��� ������ ���� ������

	/* --- �������� ---*/
	/**
	* !!! ������ ���������� �� ����������� ������������ SFML !!!
	* �������: "����� �� �������������� �������� �������, ���, ��� �� (������) ������ ������,
	* - ��� ��������� ��������� �� ��������� ��������.
	* �������������, ���� �������� ����� ���������� ��� ���������� � ������ ����� � ������,
	* ������ �������� � ������������ ���������� ��������."
	* 
	* � �� ������ ������� ������ ����� ������ ��������.
	* �� ���� ������� �������� ����� ��������� � ����� ���������� ������, � �� � ����� �� ��� �������.
	* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	**/
	sf::Texture cellTexture;
	sf::Texture livingCellTexture;
	/* --------------- */

	/* --- ������� --- */
	sf::Sprite cellSprite; // ������ ������ ������
	sf::Sprite livingCellSprite; // ������ ����� ������
	/* --------------- */

	/* ������� ���������� ����� ���� */

	/**
		@brief �������� ���� ����, ��������������� �������� ������� start() -> processing() -> end().
	*/
	void mainCycle();

	/**
		@brief ������ ����. ����� ����������� ������� ����, �������� ����.
			����� ��� ������� �������� �� ����� ������� drawing().
		@return true - ��� �������� �������� ���� �������� ���� � �������� ����
				false - ��� ���������� ������ ����������� ��� ����
	*/
	bool start(sf::RenderWindow& window);

	/**
		@brief �������� ����� ������ �� ������ ���������� ������ � ����������� ������ ����� ����.
	*/
	void processing(sf::RenderWindow& window);

	/**
		@brief ����� ����, ���������� ������, ������������ ������.
	*/
	void end(sf::RenderWindow& window);
	
	/**
		@brief ��������� ������������ ������ �������� � �������������� ��� ����� ������ �� ������ �������.
	*/
	void drawing(sf::RenderWindow& window);

	/* ��������������� ������� */

	/**
		@brief ���������� ����� ������� � ����� ��������� ������� ������.
	*/
	void pushBack(bool**& matrix);

	/**
		@brief ���������� ������� ��������� ��������� - 0 ��� 1 (� ��������� ������� ���������� - false ��� true).
	*/
	void randomFill(bool** matrix, unsigned short int x, unsigned short int y);

	/**
		@brief ������������ ������ ����������� ������� (��������� �� ��������� ����������� �� ������ ���������)
	*/
	void matrixFree(bool*** matrix, unsigned int capacity); // �������� �������
public:
	/* ������������ */
	GameLife(float updateTime = 0.05f, bool draw = false); // �� ���������������� �������� DEFAULT-��� �������������. (GameLife())
	GameLife(unsigned short int x, unsigned short int y, float updateTime, GameMode gameMode = GameMode::Default, bool draw = false);

	/* ����� ��� ����� */
	const sf::Color LIVING_CELL_COLOR = sf::Color(0, 255, 0); // ���� ����� ������
	const sf::Color IMMORTAL_CELL_COLOR = sf::Color(0, 255, 255); // ���� ������ �������� ���������� ���
                                                                  // ���������� ����������� ������ ����� N ��������.
	const sf::Color DRAWING_CELL_COLOR = sf::Color(255, 255, 0); // ���� ����� ������ � ������ ���������

	/* ���� �� �������� ���� */
	const string CELL_TEXTURE = "img/cell.png"; // ���� �� ����������� ������ ������
	const string LIVING_CELL_TEXTURE = "img/livecell.png"; // ���� �� ����������� ����� ������
};

#endif // GAME_LIFE_HPP