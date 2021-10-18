#include "SFML/Graphics.hpp"
#include <Windows.h>
#include <iostream>
#include <assert.h>
#include <set>
#include <vector>

struct node
{
	node* ParentNode;
	sf::Vector2i Pos;
	double f = 0, g = 0, h = 0;

	node(sf::Vector2i pos_)
		: Pos(pos_), ParentNode(nullptr)
	{};

	node()
		: Pos({ 0, 0 })
		, ParentNode(nullptr)
	{}
};


class astar : public sf::Drawable
{
public:
	astar(unsigned int width, unsigned int height, unsigned int w_width, unsigned int w_height)
		: m_GridWidth(width)
		, m_GridHeight(height)
		, m_WindowWidth(w_width)
		, m_WindowHeight(w_height)
		, m_NodeSize(w_width / width)
		, m_Vertices(sf::Quads)
		, m_GridVertices(sf::Lines)
		, m_MiscVertices(sf::Quads)
		, solved(false)
	{
		Init();
	}


	void Init() 
	{
		srand(time(nullptr));
		m_Grid = std::shared_ptr<node[]>(new node[m_GridWidth * m_GridHeight]);
		for (int y = 0; y < m_GridHeight; y++)
		{
			for (int x = 0; x < m_GridWidth; x++)
			{
				m_Grid[x + y * m_GridWidth].Pos.x = x;
				m_Grid[x + y * m_GridWidth].Pos.y = y;
				m_Obstacles.push_back(false);
			}
		}

		for (auto o : m_Obstacles)
		{
			o = (rand() % 5 == 0);
		}

		m_StartNode = &m_Grid[rand() % (m_GridWidth * m_GridHeight)];
		m_TargetNode = &m_Grid[rand() % (m_GridWidth * m_GridHeight)];

		m_Obstacles[m_StartNode->Pos.x + m_StartNode->Pos.y * m_GridWidth] = false;
		m_Obstacles[m_TargetNode->Pos.x + m_TargetNode->Pos.y * m_GridWidth] = false;

		std::cout << "Starting Node:" << m_StartNode->Pos.x << ", " << m_StartNode->Pos.y << ", Target Node: " << m_TargetNode->Pos.x << ", " << m_TargetNode->Pos.y << std::endl;
		m_OpenSet.push_back(m_StartNode);

		//for (int x = 1; x < m_GridWidth - 1; x++)
		//{
		//	m_Obstacles[x + 25 * m_GridWidth] = true;
		//}

		//m_Obstacles[15] = true;
		//m_Obstacles[27] = true;
		//m_Obstacles[28] = true;
		//m_Obstacles[29] = true;
		//m_Obstacles[30] = true;
		//m_Obstacles[31] = true;


	}


	void SolveStep()
	{
		if (!m_OpenSet.empty() && !solved)
		{
			node* current = m_OpenSet[GetBestIndex()];
			if (current == m_TargetNode)
			{
				std::cout << "Found Target" << std::endl;
				BackTrack();
				DrawPath();
				solved = true;
				return;
			}
			m_OpenSet.erase(m_OpenSet.begin() + GetBestIndex());
			m_ClosedSet.push_back(current);


			for (int dy = -1; dy <= 1; dy++)
			{
				for (int dx = -1; dx <= 1; dx++)
				{
					if (!(dx == 0 && dy == 0))
					{
						if ((current->Pos.x + dx < m_GridWidth && current->Pos.x + dx >= 0) && (current->Pos.y + dy < m_GridHeight && current->Pos.y + dy >= 0) && !m_Obstacles[current->Pos.x + dx + (current->Pos.y + dy) * m_GridWidth])
						{
							node& succesor = m_Grid[current->Pos.x + dx + (current->Pos.y + dy) * m_GridWidth];
							int tentative_g = current->g + ((current->Pos.x != succesor.Pos.x && current->Pos.y != succesor.Pos.y) ? D2 : D);
							if (!IsInOpenSet(succesor.Pos) && !IsInClosedSet(succesor.Pos))
							{
								succesor.g = tentative_g;
								succesor.h = CalculateHeuristic(succesor);
								succesor.f = succesor.g + succesor.h;
								succesor.ParentNode = current;
								m_OpenSet.push_back(&succesor);
							}
							else if (IsInOpenSet(succesor.Pos) &&  tentative_g < succesor.g)
							{
								succesor.ParentNode = current;
								succesor.g = tentative_g;
								succesor.f = succesor.g + succesor.h;
							}
						}
					}
				}
			}
		}
	}

	int GetBestIndex()
	{
		int best_index = -1;
		for (int i = 0; i < m_OpenSet.size(); i++)
		{
			if (best_index == -1 || m_OpenSet[i]->f < m_OpenSet[best_index]->f)
			{
				best_index = i;
			}
		}
		return best_index;
	}

	bool IsInOpenSet(sf::Vector2i pos)
	{
		bool ret = false;
		for (int i = 0; i < m_OpenSet.size(); i++)
		{
			if (m_OpenSet[i]->Pos == pos)
			{
				ret = true;
					break;
			}
		}

		return ret;
	}

	bool IsInClosedSet(sf::Vector2i pos)
	{
		bool ret = false;
		for (int i = 0; i < m_ClosedSet.size(); i++)
		{
			if (m_ClosedSet[i]->Pos == pos)
			{
				ret = true;
				break;
			}
		}

		return ret;
	}

	int CalculateHeuristic(node& n) const
	{
		//return 0;
		/*Diagonal Distance*/
		int dx = abs(n.Pos.x - m_TargetNode->Pos.x);
		int dy = abs(n.Pos.y - m_TargetNode->Pos.y);
		return D * (dx + dy) + (D2 - 2 * D) * ((dx < dy) ? dx : dy);

		/*Manhattan Distance*/
		//return abs(n.Pos.x - m_TargetNode->Pos.x)*D + abs(n.Pos.y - m_TargetNode->Pos.y)*D;

		/*Euclidian Distance*/
		//return sqrt( (n.Pos.x - m_TargetNode->Pos.x) * (n.Pos.x - m_TargetNode->Pos.x) + (n.Pos.y - m_TargetNode->Pos.y) * (n.Pos.y - m_TargetNode->Pos.y));

		/* Quint Distance*/
		//return (n.Pos.x - m_TargetNode->Pos.x) + (n.Pos.y - m_TargetNode->Pos.y);
	}

	void BackTrack()
	{
		node* current = m_TargetNode->ParentNode;
		while (current != m_StartNode)
		{
			m_FinishedPath.push_back(current->Pos);
			current = current->ParentNode;
		}
	}


	void UpdateVertices()
	{
		m_Vertices.clear();


		for (node* n : m_ClosedSet)
		{
			DrawNode(n->Pos.x, n->Pos.y, sf::Color(87, 151, 255));
		}

		for (node* n : m_OpenSet)
		{
			DrawNode(n->Pos.x, n->Pos.y, sf::Color(64, 173, 70));
		}

		DrawNode(m_StartNode->Pos.x, m_StartNode->Pos.y, sf::Color(255, 249, 87));
		DrawNode(m_TargetNode->Pos.x, m_TargetNode->Pos.y, sf::Color(255, 87, 230));
	}


	void DrawNode(unsigned int x, unsigned int y, sf::Color clr)
	{
		m_Vertices.append(sf::Vertex(sf::Vector2f(x * m_NodeSize + m_GRIDSPACING, y * m_NodeSize + m_GRIDSPACING), clr));
		m_Vertices.append(sf::Vertex(sf::Vector2f((x + 1) * m_NodeSize - m_GRIDSPACING, y * m_NodeSize + m_GRIDSPACING), clr));
		m_Vertices.append(sf::Vertex(sf::Vector2f((x + 1) * m_NodeSize - m_GRIDSPACING, (y + 1) * m_NodeSize - m_GRIDSPACING), clr));
		m_Vertices.append(sf::Vertex(sf::Vector2f(x * m_NodeSize + m_GRIDSPACING, (y + 1) * m_NodeSize - m_GRIDSPACING), clr));
	}


	void DrawMisc(unsigned int x, unsigned int y, sf::Color clr)
	{
		m_MiscVertices.append(sf::Vertex(sf::Vector2f(x * m_NodeSize + m_GRIDSPACING, y * m_NodeSize + m_GRIDSPACING), clr));
		m_MiscVertices.append(sf::Vertex(sf::Vector2f((x + 1) * m_NodeSize - m_GRIDSPACING, y * m_NodeSize + m_GRIDSPACING), clr));
		m_MiscVertices.append(sf::Vertex(sf::Vector2f((x + 1) * m_NodeSize - m_GRIDSPACING, (y + 1) * m_NodeSize - m_GRIDSPACING), clr));
		m_MiscVertices.append(sf::Vertex(sf::Vector2f(x * m_NodeSize + m_GRIDSPACING, (y + 1) * m_NodeSize - m_GRIDSPACING), clr));
	}


	void InitVertices()
	{
		for (int y = 0; y < m_GridHeight; y++)
		{
			m_GridVertices.append(sf::Vertex(sf::Vector2f(0, y * m_NodeSize), sf::Color::Black));
			m_GridVertices.append(sf::Vertex(sf::Vector2f(m_WindowWidth, y * m_NodeSize), sf::Color::Black));
		}

		for (int x = 0; x < m_GridWidth; x++)
		{
			m_GridVertices.append(sf::Vertex(sf::Vector2f(x * m_NodeSize, 0), sf::Color::Black));
			m_GridVertices.append(sf::Vertex(sf::Vector2f(x * m_NodeSize, m_WindowHeight), sf::Color::Black));
		}


		for (int y = 0; y < m_GridHeight; y++)
		{
			for (int x = 0; x < m_GridWidth; x++)
			{
				if (m_Obstacles[x + y * m_GridWidth])
					DrawMisc(x, y, sf::Color::Black);
			}
		}
	}


	void DrawPath()
	{
		for (auto& v : m_FinishedPath)
		{
			DrawMisc(v.x, v.y, sf::Color::Green);
		}
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const 
	{
		target.draw(m_Vertices, states);
		target.draw(m_MiscVertices, states);
		target.draw(m_GridVertices, states);
	}


private:

	const int D = 10;
	const int D2 = 14;

	const unsigned int m_GRIDSPACING = 0;

	size_t m_GridWidth, m_GridHeight;
	size_t m_WindowWidth, m_WindowHeight;
	unsigned int m_NodeSize; //size of nodes in pixels;
	node* m_StartNode;
	node* m_TargetNode;
	std::shared_ptr<node[]> m_Grid;
	std::vector<node*> m_ClosedSet;
	std::vector<node*> m_OpenSet;
	std::vector<bool> m_Obstacles;
	std::vector<sf::Vector2i> m_FinishedPath;
	sf::VertexArray m_Vertices;		//Vertex Array for openset, closedset, startnode and targetnode
	sf::VertexArray m_GridVertices; //Vertex Array for gridlines
	sf::VertexArray m_MiscVertices; //Vertex Array for obstacles and final path

	bool solved;
};
