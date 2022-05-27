#include "HLMesh.h"

void HLMesh::reconstructVertexArray()
{
    vertices.clear();
    for (auto f : faces) {
        Utils::addAllUnique(vertices, f.getVertices());
    }
}

HLMesh HLMesh::fromMesh(Render::Mesh mesh, bool quads)
{
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
        for (auto j = 0; j < inc; j++) {
            auto first = i + j, second = i + ((j + 1) % (inc));
            auto firstValue = mesh.index_array[first], secondValue = mesh.index_array[second];
            Loggger::debug("Adding %i and %i edge", first, second);
            edges.push_back({
                mesh.vertex_array[firstValue],
                mesh.vertex_array[secondValue]
            });
        }
        hl.faces.push_back(Face{ edges });
    }

    return hl;
}

void HLMesh::addFace(Face f)
{
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
    reconstructVertexArray();
    mesh.vertex_array = vertices;

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
    /*
    if (size != 3) {
        Loggger::error("Face cannot be created from %i vertices", size);
        return;
    }
    */

    for (auto i = 0; i < size; i++) {
        edges.push_back({ verts[i], verts[(i + 1) % size]});
    }
}

std::vector<Vertex> HLMesh::Face::getVertices()
{
    std::vector<Vertex> verts;

    for (auto el : edges)
    {
        Utils::addUnique(verts, el.vertex0);
        Utils::addUnique(verts, el.vertex1);
    }

    return verts;
}
