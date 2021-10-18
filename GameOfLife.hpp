#pragma once

#include "SFML/Graphics.hpp"
#include <random>
#include <cstdlib>
#include <iostream>
#include <vector>

struct Node
{
	bool state;
	int nCount;

	//Node() : state(false), nCount(0) {};
};

class GOL
{
private: 
	const int m_GRIDSPACING = 0;

	std::vector<Node> m_Grid;
	std::vector<Node> m_NextGrid;


	int m_GridWidth, m_GridHeight;
	int m_WindowWidth, m_WindowHeight;


	int m_Length;


public:
	sf::VertexArray m_Vertices;

	GOL(int width, int height, int w_width, int w_height)
		: m_GridWidth(width)
		, m_GridHeight(height)
		, m_Length(width * height)
		, m_WindowWidth(w_width)
		, m_WindowHeight(w_height)
		, m_Vertices(sf::Quads)
	{
		m_Grid.reserve(m_Length);
		m_NextGrid.reserve(m_Length);
		//std::cout << "Length: " << m_Length << std::endl;
	};

	void Init()
	{
		for (int i = 0; i < m_Length; i++)
		{
			m_Grid.push_back(Node());
			m_NextGrid.push_back(Node());
		}
	}

	void NextGen()
	{
		for (size_t y = 0; y < m_GridWidth; y++)
		{
			for (size_t x = 0; x < m_GridHeight; x++)
			{
				if (m_Grid[x + y * m_GridWidth].state || m_Grid[x + y * m_GridWidth].nCount)
				{

					if (m_Grid[x + y * m_GridWidth].state)
					{
						DrawCell(x, y, { 0, 0, 0 });
						if (m_Grid[x + y * m_GridWidth].nCount < 2)
						{
							ClearCell(x, y);
						}
						if (m_Grid[x + y * m_GridWidth].nCount > 3)
						{
							ClearCell(x, y);
						}

					}
					else if (m_Grid[x + y * m_GridWidth].nCount == 3)
					{
						SetCell(x, y);
					}

				}

			}
		}
		
		SwapGrids();
	}

	void ClearVertices()
	{
		m_Vertices.clear();
	}

	void SetCell(int x, int y)
	{
		m_NextGrid[y * m_GridWidth + x].state = true;

		int newx = 0, newy = 0;
		for (int dy = -1; dy <= 1; dy++)
		{
			for (int dx = -1; dx <= 1; dx++)
			{
				if (!(dx == 0 && dy == 0))
				{
					if (x + dx < 0)
						newx = m_GridWidth - 1;
					else if (x + dx > m_GridWidth-1)
						newx = 0;
					else
						newx = x + dx;
					if (y + dy < 0)
						newy = m_GridHeight - 1;
					else if (y + dy > m_GridHeight-1)
						newy = 0;
					else
						newy = y + dy;
					m_NextGrid[newy * m_GridWidth + newx].nCount++;
					
				}
			}
		}


	}

	void ClearCell(int x, int y)
	{
		m_NextGrid[y * m_GridWidth + x].state = false;

		int newx = 0, newy = 0;
		for (int dy = -1; dy <= 1; dy++)
		{
			for (int dx = -1; dx <= 1; dx++)
			{
				if (!(dx == 0 && dy == 0))
				{
					if (x + dx < 0)
						newx = m_GridWidth - 1;
					else if (x + dx > m_GridWidth - 1)
						newx = 0;
					else
						newx = x + dx;
					if (y + dy < 0)
						newy = m_GridHeight - 1;
					else if (y + dy > m_GridHeight - 1)
						newy = 0;
					else
						newy = y + dy;

					m_NextGrid[newy * m_GridWidth + newx].nCount--;

				}
			}
		}
	}

	void DrawCell(unsigned int x, unsigned int y, sf::Color clr)
	{
		float nodeWidth = m_WindowWidth / m_GridWidth;
		float nodeHeight = m_WindowHeight / m_GridHeight;

		m_Vertices.append(sf::Vertex(sf::Vector2f(x * nodeWidth + m_GRIDSPACING, y * nodeHeight + m_GRIDSPACING), clr));
		m_Vertices.append(sf::Vertex(sf::Vector2f((x + 1) * nodeWidth - m_GRIDSPACING, y * nodeHeight + m_GRIDSPACING), clr));
		m_Vertices.append(sf::Vertex(sf::Vector2f((x + 1) * nodeWidth - m_GRIDSPACING, (y + 1)  * nodeHeight - m_GRIDSPACING), clr));
		m_Vertices.append(sf::Vertex(sf::Vector2f(x * nodeWidth + m_GRIDSPACING, (y + 1) * nodeHeight - m_GRIDSPACING), clr));
	}

	void ImportStringMap(const char* map, int m_w, int m_h)
	{
		const int OffsetX = 10;
		const int OffsetY = 10;
		for (int y = 0; y < m_h; y++)
		{
			for (int x = 0; x < m_w; x++)
			{
				if (map[x + y * m_w] - 48)
				{
					SetCell(x + OffsetX, y + OffsetY);
				}
			}
		}
	}

	void InitRandom()
	{
		for (size_t y = 0; y < m_GridWidth; y++)
		{
			for (size_t x = 0; x < m_GridHeight; x++)
			{
				if (std::rand() % 2)
				{
					SetCell(x, y);
				}
			}
		}
	}

	void SwapGrids()
	{
		auto grid = m_Grid;
		m_Grid = m_NextGrid;
		m_NextGrid = m_Grid;

	}

	void PrintGrid()
	{
		for (int y = 0; y < m_GridHeight; y++)
		{
			for (int x = 0; x < m_GridWidth; x++)
			{
				std::cout << "(x, y, state, nCount): " << x << ", " << y << ", " << m_Grid[x + (size_t)y * m_GridWidth].state << ", " << m_Grid[x + (size_t)y * m_GridWidth].nCount << std::endl;
			}
		}
	}
};