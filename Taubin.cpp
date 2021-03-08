#include "Taubin.h"

void Taubin(Triangulation& t, double lambda, double mu, int iterations)
{
	std::map<int, std::set<int>> neighbours;

	// filling map of neighbours
	for (int i = 0; i < t.vertices.size(); ++i)
	{
		//Triangulation::Vertex& v = t.vertices[i];
		for (int j = 0; j < t.triangles.size(); ++j)
		{
			Triangulation::Triangle& triangle = t.triangles[j];
			for (int k = 0; k < 3; ++k)
			{
				if (triangle.indices[k] == i)
				{
					neighbours[i].insert(triangle.indices[(k + 1) % 3]);
					neighbours[i].insert(triangle.indices[(k + 2) % 3]);
				}
			}
		}
	}

	for (int it = 0; it < iterations; ++it)
	{
		Update(t, neighbours, lambda);
		Update(t, neighbours, mu);
	}
	
	return;
}

void Update(Triangulation& t, std::map<int, std::set<int>>& neighbours, double factor)
{
	for (int i = 0; i < t.vertices.size(); ++i)
	{
		double weight = 1. / neighbours[i].size();
		R3Graph::R3Vector average_vector = { 0., 0., 0. };

		for (const auto ind : neighbours[i])
			average_vector += (t.vertices[ind].point - t.vertices[i].point) * weight;


		t.vertices[i].point += average_vector * factor;
	}
}