#include "STL.h"
#include "R3Graph.h"
#include <fstream>
#include <iomanip>

void WriteSTLBinary(const Triangulation& triangulation, std::string& filename)
{
	std::ofstream out;
	// Check path after merging
	//filename = "C:\\Users\\owchi\\source\\repos\\Triangulation\\bin64\\" + filename;
	out.open(filename, std::ios::out | std::ios::binary);
	if (out.is_open())
	{
		char header[80];
		std::memset(header, 0, 80);
		out.write(header, 80);

		auto numTriang = triangulation.triangles.size();
		out.write((const char*)(&numTriang), 4);


		for (int i = 0; i < triangulation.triangles.size(); ++i)
		{
			const Triangulation::Triangle& t = triangulation.triangles.at(i);

			R3Graph::R3Point p[4];
			p[0] = t.Normal;
			p[1] = triangulation.vertices.at(t.indices[0]).point;
			p[2] = triangulation.vertices.at(t.indices[1]).point;
			p[3] = triangulation.vertices.at(t.indices[2]).point;
				

			for (int i = 0; i < 4; ++i)
			{
				float vec[3];
				vec[0] = (float)p[i].x;
				vec[1] = (float)p[i].y;
				vec[2] = (float)p[i].z;
				out.write((const char*)&vec[0], 4);
				out.write((const char*)&vec[1], 4);
				out.write((const char*)&vec[2], 4);
			}

			unsigned short dummy = 0;
			out.write((const char*)&dummy, 2);
		}
	}
	out.close();
}

void WriteStlASCII(const Triangulation& triangulation, std::string& filename)
{
	std::ofstream out;
	out.open(filename);
	if (out.is_open())
	{
		out << "solid iso" << std::endl;

		for (int i = 0; i < triangulation.triangles.size(); ++i) 
		{
			out << "\t" << "facet normal ";

			Triangulation::Triangle t = triangulation.triangles.at(i);

			R3Graph::R3Point p0 = triangulation.vertices.at(t.indices[0]).point;
			R3Graph::R3Point p1 = triangulation.vertices.at(t.indices[1]).point;
			R3Graph::R3Point p2 = triangulation.vertices.at(t.indices[2]).point;

			//out << std::setprecision(6);
			out << std::setprecision(9) << t.Normal.x << " " << t.Normal.y << " " << t.Normal.z << std::endl;
			out << "\t" << "outer loop" << std::endl;

			//out << std::setprecision(6);
			out << "\t\t" << "vertex " << std::setprecision(9) << p0.x << " " << p0.y << " " << p0.z << std::endl;
			out << "\t\t" << "vertex " << std::setprecision(9) << p1.x << " " << p1.y << " " << p1.z << std::endl;
			out << "\t\t" << "vertex " << std::setprecision(9) << p2.x << " " << p2.y << " " << p2.z << std::endl;

			out << "\t" << "endloop" << std::endl;
			out << "\t" << "endfacet" << std::endl;
		}

		out << "endsolid iso";
	}
	out.close();
}

