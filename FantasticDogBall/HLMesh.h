#pragma once

#include <vector>
#include <map>

#include "Vertex.h"
#include "Render.h"

class HLMesh
{
public:
	static HLMesh fromMesh(Render::Mesh mesh, bool quads = false);

	struct Edge {
		Vertex vertex0, vertex1;

		friend bool operator==(const Edge& e0, const Edge& e1) {
			return e0.vertex0 == e1.vertex0
				&& e0.vertex1 == e1.vertex1;
		}

		std::vector<Vertex> getVertices() const;
		std::string toString() const;
	};

	struct Face {
		std::vector<Edge> edges;

		Face() = default;
		Face(std::vector<Edge> edges);
		Face(std::vector<Vertex> verts);

		friend bool operator==(const Face& f0, const Face& f1) {
			return f0.edges == f1.edges;
		};

		std::vector<Vertex> getVertices() const;
		bool hasEdge(Edge e) const;
		bool hasVertex(Vertex v) const;

		std::string toString() const;
	};
	std::vector<Vertex> vertices;
	std::vector<Face> faces;

	HLMesh() = default;

	void addFace(Face f);
	void removeFace(int index);
	void removeFace(Face f);
	void reconstructVertexArray();
	std::vector<Face> getFacesOnEdge(Edge e);
	std::vector<Vertex> getNeighbors(Vertex v);
	std::vector<Vertex> getVertices();
	Render::Mesh toMesh();

private:
};

