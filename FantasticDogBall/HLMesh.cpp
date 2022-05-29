#include "HLMesh.h"

void HLMesh::reconstructVertexArray()
{
    vertices.clear();
    for (auto f : faces) {
        Utils::addAllUnique(vertices, f.getVertices());
    }
}

std::vector<HLMesh::Face> HLMesh::getFacesOnEdge(Edge e)
{
    std::vector<Face> edgeFaces;
    for (const Face& f : faces)
        if (f.hasEdge(e))
            edgeFaces.push_back(f);
    return edgeFaces;
}

std::vector<Vertex> HLMesh::getNeighbors(Vertex v)
{
    std::vector<Vertex> neighbors;

    for (const Face& f : faces) {
        if (f.hasVertex(v)) {
            for (const Edge& e : f.edges) {
                if (e.vertex0 == v)
                    Utils::addUnique(neighbors, e.vertex1);
                else if (e.vertex1 == v)
                    Utils::addUnique(neighbors, e.vertex0);
            }
        }
    }

    return neighbors;
}

std::vector<Vertex> HLMesh::getVertices()
{
    std::vector<Vertex> verts;
    for (auto& f : faces)
        Utils::addAllUnique(verts, f.getVertices());
    return verts;
}

HLMesh HLMesh::fromMesh(Render::Mesh mesh, bool quads)
{
    // at the moment we do not support quads, maybe never
    quads = false;
    unsigned short inc = quads ? 4 : 3;
    HLMesh hl;
    if (mesh.index_array.size() % inc != 0) {
        Loggger::error("Mesh cannot be converted to %s High-Level mesh, index array is not divisible by %i", quads ? "Quads" : "Triangles", inc);
        return hl;
    }
    hl.vertices = mesh.vertex_array;
    for (auto i = 0; i < mesh.index_array.size(); i+=inc) {
        std::vector<Edge> edges;
        std::vector<Vertex> verts;
        for (auto j = 0; j < inc; j++) {
            auto first = i + j, second = i + ((j + 1) % (inc));
            auto firstValue = mesh.index_array[first], secondValue = mesh.index_array[second];
            Loggger::trace("Adding %i and %i edge", first, second);
            edges.push_back({
                mesh.vertex_array[firstValue],
                mesh.vertex_array[secondValue]
            });
        }
        Face face = Face{ edges };
        hl.faces.push_back(face);
    }

    return hl;
}

void HLMesh::addFace(Face f)
{
    Loggger::trace("Adding face %s", f.toString().c_str());
    Utils::addAllUnique(vertices, f.getVertices());
    bool added = Utils::addUnique(faces, f);
}

void HLMesh::removeFace(int index)
{
    if (index < 0 || index >= faces.size()) {
        Loggger::warn("Face at index %i cannot be removed", index);
    }

    Utils::remove(faces, index);
    // lazy and inefficient but effective
    reconstructVertexArray();
}

void HLMesh::removeFace(Face f)
{
    int index = Utils::getIndex(faces, f);
    if (index < 0) {
        Loggger::warn("Face %s could not be found", f);
        return;
    }
    removeFace(index);
}

Render::Mesh HLMesh::toMesh()
{
    Render::Mesh mesh;
    mesh.vertex_array = getVertices();

    for (auto i = 0; i < faces.size(); i++) {
        Face face = faces[i];
        for (auto j = 0; j < face.edges.size(); j++) {
            Vertex v = face.edges[j].vertex0;
            unsigned index = Utils::getIndex(mesh.vertex_array, v);
            if(index == -1) Loggger::error("index -1");
            mesh.index_array.push_back(index);
        }
    }

    return mesh;
}

HLMesh::Face::Face(std::vector<Edge> edges) : edges(edges)
{
}

HLMesh::Face::Face(std::vector<Vertex> verts)
{
    unsigned size = verts.size();
    if (size != 3) {
        Loggger::error("Face cannot be created from %i vertices", size);
        return;
    }

    for (auto i = 0; i < size; i++) {
        edges.push_back({ verts[i], verts[(i + 1) % size]});
    }
}

std::vector<Vertex> HLMesh::Face::getVertices() const
{
    std::vector<Vertex> verts;

    for (const HLMesh::Edge& el : edges)
    {
        Utils::addUnique(verts, el.vertex0);
        Utils::addUnique(verts, el.vertex1);
    }

    return verts;
}

bool HLMesh::Face::hasEdge(Edge e) const
{
    for (const Edge& edge : edges)
        if (e == edge)
            return true;
}

bool HLMesh::Face::hasVertex(Vertex v) const
{
    for (const auto& e : edges)
        if (e.vertex0 == v || e.vertex1 == v)
            return true;

    return false;
}

std::string HLMesh::Face::toString() const
{
    std::string s;

    for (const auto& e : edges) {
        s += Utils::string_format("%s,", e.toString().c_str());
    }

    return s;
}

std::vector<Vertex> HLMesh::Edge::getVertices() const
{
    return std::vector<Vertex>({ vertex0, vertex1 });
}

std::string HLMesh::Edge::toString() const
{
    return Utils::string_format("(%s, %s)", vertex0.toString().c_str(), vertex1.toString().c_str());
}
