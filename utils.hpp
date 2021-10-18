#pragma once

namespace utils {
	float lerp(float v0, float v1, float t) //relatively fast
	{
		return v0 + t * (v1 - v0);
	}

	float better_lerp(float v0, float v1, float t) //relatively slow
	{
		return (1 - t) * v0 + t * v1;
	}

	//const void PrintNode(Node& n)
	//{
	//	std::cout << "	-Node (x, y | f, g, h): " << n.Pos.x << ", " << n.Pos.y << " | " << n.f << ", " << n.g << ", " << n.h << " | At adress: " << &n << std::endl;
	//}


	//void PrintPQ(std::priority_queue<Node*, std::vector<Node*>, NodeHeuristicsCompare> q)
	//{
	//	std::cout << "Open Set: " << std::endl;
	//	while (!q.empty()) {
	//		PrintNode(*(q.top()));
	//		q.pop();
	//	}
	//}

	//void PrintUS(std::unordered_set<Node*> const& s)
	//{
	//	std::cout << "Closed Set: " << std::endl;
	//	for (auto const& i : s) {
	//		PrintNode(*i);
	//	}
	//}

	int Flatten2DIndex(int x, int y, int width)
	{
		return (x * width + y);
	}


}