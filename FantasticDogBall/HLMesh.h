#pragma once

#include <vector>
#include "Vertex.h"
#include "Render.h"

class HLMesh
{
private:
	HLMesh() = default;

	void reconstructVertexArray();
public:
	static HLMesh fromMesh(Render::Mesh mesh, bool quads = false);

	struct Edge {
		Vertex vertex0, vertex1;

		friend bool operator==(const Edge& e0, const Edge& e1) {
			return e0.vertex0 == e1.vertex0
				&& e0.vertex1 == e1.vertex1;
		}
	};

	struct Face {
		std::vector<Edge> edges;

		friend bool operator==(const Face& f0, const Face& f1) {
			return f0.edges == f1.edges;
		};

		std::vector<Vertex> getVertices();
	};
	std::vector<Vertex> vertices;
	std::vector<Face> faces;

	void addFace(Face f);
	void removeFace(int index);
	void removeFace(Face f);
	Render::Mesh toMesh();

};

