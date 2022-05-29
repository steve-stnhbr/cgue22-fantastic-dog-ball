#include "RenderObject.h"

#include <memory>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btMotionState.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <GL/glew.h>
#include <future>

#include "Utils.h"
#include "HLMesh.h"

RenderObject::RenderObject(Render::Mesh mesh_, Material::Material* material_, const std::string& name_) :
	mesh(mesh_),
	material(material_),
	name(name_),
	transform(glm::mat4(1))
{
	buildVAO();
}

void RenderObject::update()
{
}


RenderObject* RenderObject::add(Decoration::Decoration& decoration_)
{
	decoration_.bind(this);
	// decorations[typeid(decoration_)] = decoration_;
	return this;
}

void RenderObject::buildVAO() const
{
	glCreateBuffers(1, (GLuint*) &vboID);
	glNamedBufferStorage(vboID, mesh.vertex_array.size() * sizeof(Vertex), mesh.vertex_array.data(), GL_DYNAMIC_STORAGE_BIT);
	Utils::checkError();

	glCreateBuffers(1, (GLuint*)&eboID);
	glNamedBufferStorage(eboID, mesh.index_array.size() * sizeof(unsigned), mesh.index_array.data(), GL_DYNAMIC_STORAGE_BIT);
	Utils::checkError();

	using vertexType = std::decay_t<decltype(*mesh.vertex_array.begin())>;
	using indexType = std::decay_t<decltype(*mesh.index_array.begin())>;

	glCreateVertexArrays(1, (GLuint*)&vaoID);
	glVertexArrayVertexBuffer(vaoID, 0, vboID, 0, sizeof(vertexType));
	glVertexArrayElementBuffer(vaoID, eboID);
	
	material->assignVertexAttributes(vaoID);
}

RenderObject* RenderObject::scale(float s)
{
	scale({ s,s,s });
	return this;
}

RenderObject* RenderObject::scale(float x, float y, float z)
{
	scale({ x,y,z });
	return this;
}

RenderObject* RenderObject::scale(glm::vec3 s)
{
	transform = glm::scale(transform, s);
	return this;
}

void RenderObject::doTransform()
{

}

RenderObject* RenderObject::translate(float x, float y, float z)
{
	translate({ x, y, z });
	return this;
}

RenderObject* RenderObject::translate(glm::vec3 v)
{
	transform = glm::translate(transform, v);
	return this;
}

RenderObject* RenderObject::rotate(float x, float y, float z)
{
	rotate(x, { 1, 0, 0 });
	rotate(y, { 0, 1, 0 });
	rotate(z, { 0, 0, 1 });
	return this;
}

RenderObject* RenderObject::rotate(float angle, glm::vec3 axes)
{
	transform = glm::rotate(transform, angle, axes);
	return this;
}


void Decoration::Decoration::bind(RenderObject* object_)
{
	object = object_;
	onBind(object_);
}

void Decoration::Physics::update(unsigned frame, float dTime)
{
}

void Decoration::Physics::onBind(RenderObject*)
{
}

Decoration::Compute::ComputeType::ComputeType(Shaders::Program program_) : program(program_)
{
}

Decoration::Compute::CatmullClarkSubdivision::CatmullClarkSubdivision() : CatmullClarkSubdivision(1)
{
}

Decoration::Compute::CatmullClarkSubdivision::CatmullClarkSubdivision(unsigned short levels_)
{
	isGeometry = true;
	if (levels > 3)
		Loggger::warn("Catmull-Clark subdivision was called with %us levels. The computation may take a while", levels);
	levels = levels_;
}

void Decoration::Compute::CatmullClarkSubdivision::doCompute(RenderObject*)
{
}

HLMesh Decoration::Compute::SimpleSubdivision::subdivide(HLMesh hl)
{
	HLMesh newMesh;
	for (const auto& face : hl.faces) {
		std::vector<Vertex> verts;
		for (const auto& edge : face.edges) {
			verts.push_back(edge.vertex0);
			Vertex halfway = Vertex::halfway(edge.vertex0, edge.vertex1);
			verts.push_back(halfway);
		}

		newMesh.addFace({ { verts[0], verts[1], verts[5] } });
		newMesh.addFace({ { verts[1], verts[2], verts[3] } });
		newMesh.addFace({ { verts[3], verts[4], verts[5] } });
		newMesh.addFace({ { verts[1], verts[3], verts[5] } });
	}
	return newMesh;
}

std::vector<HLMesh::Face> subdivisionAsyncSubstep(std::vector<HLMesh::Face> faces) {
	std::vector<HLMesh::Face> newFaces;
	for (const auto& face : faces) {
		std::vector<Vertex> verts;
		for (const auto& edge : face.edges) {
			verts.push_back(edge.vertex0);
			Vertex halfway = Vertex::halfway(edge.vertex0, edge.vertex1);
			halfway.color = glm::vec4(1);
			verts.push_back(halfway);
			verts.push_back(edge.vertex1);
		}

		newFaces.push_back({ { verts[0], verts[1], verts[5] } });
		newFaces.push_back({ { verts[1], verts[2], verts[3] } });
		newFaces.push_back({ { verts[3], verts[4], verts[5] } });
		newFaces.push_back({ { verts[1], verts[3], verts[5] } });
	}

	return newFaces;
}


HLMesh Decoration::Compute::SimpleSubdivision::subdivideAsync(HLMesh hl)
{
	HLMesh newMesh;
	std::vector<std::vector<HLMesh::Face>> faces = Utils::splitNWays(hl.faces, Utils::getLogicalCores() - 1);
	std::vector<std::future<std::vector<HLMesh::Face>>> futures;
	for (auto i = 0; i < faces.size(); i++) {
		auto fut = std::async(std::launch::async, subdivisionAsyncSubstep, faces[i]);
		futures.push_back(std::move(fut));
	}

	for (auto& fut : futures) {
		auto get = fut.get();
		newMesh.faces.insert(newMesh.faces.end(), get.begin(), get.end());
	}
	return newMesh;
}

Decoration::Compute::SimpleSubdivision::SimpleSubdivision() : SimpleSubdivision(1)
{
}

Decoration::Compute::SimpleSubdivision::SimpleSubdivision(unsigned short levels_)
{
	isGeometry = true;
	if (levels > 5)
		Loggger::warn("Simple subdivision was called with %us levels. The computation may take a while", levels);
	levels = levels_;
}

void Decoration::Compute::SimpleSubdivision::doCompute(RenderObject* obj)
{

	HLMesh hl = HLMesh::fromMesh(obj->mesh);
	Loggger::debug("Simple subdivision with %u levels called on %l faces\nThis will result in %l faces", levels, hl.faces.size(), hl.faces.size() * pow(2, levels));
	for (auto i = 0; i < levels; i++) {
		hl = subdivide(hl);
	}
	obj->mesh = hl.toMesh();
}

Decoration::Compute::Compute(ComputeType* type) : Compute(std::vector<ComputeType*>{type})
{
}

Decoration::Compute::Compute(std::vector<ComputeType*> types_) : types(types_)
{
}

void Decoration::Compute::update(unsigned frame, float dTime)
{
}

void Decoration::Compute::onBind(RenderObject* obj)
{
	for (ComputeType* type : types) {
		type->doCompute(obj);
	}

	obj->buildVAO();
}

void Decoration::Animation::onBind(RenderObject*)
{
}

Decoration::Compute::LoopSubdivision::LoopVertex::LoopVertex(Vertex v, bool even) : Vertex(v), even(even)
{
}

HLMesh Decoration::Compute::LoopSubdivision::subdivide(HLMesh hl)
{
	HLMesh newMesh;
	for (const auto& face : hl.faces) {
		std::vector<LoopVertex> verts;
		for (const auto& edge : face.edges) {
			auto faces = hl.getFacesOnEdge(edge);
			auto otherFace = faces[0] == face ? faces[1] : faces[0];
			auto verticesOnEdge = edge.getVertices();
			auto verticesNotOnEdge = Utils::except(Utils::unionUnique(face.getVertices(), otherFace.getVertices()), verticesOnEdge);
			Vertex newVert;
			if (verticesNotOnEdge.size() < 2) {
				newVert = (verticesOnEdge[0] + verticesOnEdge[1]) * 1 / 2;
			} else {
				newVert = (verticesOnEdge[0] + verticesOnEdge[1]) * 3 / 8 + (verticesNotOnEdge[0] + verticesNotOnEdge[1]) * 1 / 8;
			}
			newVert.color = glm::vec4(-1);
			
			Utils::addUnique(verts, LoopVertex(edge.vertex0, true));
			Utils::addUnique(verts, LoopVertex(edge.vertex1, true));
			verts.push_back(LoopVertex(newVert, false));
		}

		newMesh.addFace({ { verts[0], verts[1], verts[5] } });
		newMesh.addFace({ { verts[1], verts[2], verts[3] } });
		newMesh.addFace({ { verts[3], verts[4], verts[5] } });
		newMesh.addFace({ { verts[1], verts[3], verts[5] } });
	}

	for (auto& face : newMesh.faces) {
		for (auto& edge : face.edges) {
			auto& v = edge.vertex0;
			if (v.color[0] == -1) continue;
			const auto index = Utils::getIndex(newMesh.vertices, v);
			auto neighbors = newMesh.getNeighbors(v);
			auto n = neighbors.size();
			float u;
			if (n < 3) {
				//const float u = (5.0 / 8.0 - pow(3.0 / 8.0 + 1.0 / 4.0 * std::cos(SIMD_2_PI / n), 2)) / n;
				u = 3.0f / 8.0f * n;
			}
			else {
				/*
				auto sum = neighbors[0] + neighbors[1];
				newV = v * 0.75 + sum * 0.125;
				*/
				u = 0.1875f;
			}
			Vertex sum = Vertex();
			for (const auto& n : neighbors)
				sum = sum + n;
			//auto newV = v * (1 - n * u) + sum * u;
			auto newV = v * (1.0f - n * u) + sum * u;
			v.position = newV.position;
			v.color = newV.color;
			v.texture_coordinate = newV.texture_coordinate;
			v.normal = newV.normal;
		}
	}

	return newMesh;
}

Decoration::Compute::LoopSubdivision::LoopSubdivision() : LoopSubdivision(1) {}

Decoration::Compute::LoopSubdivision::LoopSubdivision(unsigned short levels_) {
	isGeometry = true;
	if (levels > 3)
		Loggger::warn("Loop subdivision was called with %us levels. The computation may take a while", levels);
	levels = levels_;
}

void Decoration::Compute::LoopSubdivision::doCompute(RenderObject* obj)
{
	HLMesh hl = HLMesh::fromMesh(obj->mesh);
	Loggger::debug("Loop subdivision with %u levels called on %u faces\nThis will result in %u faces", levels, hl.faces.size(), (hl.faces.size() * pow(2, levels)));
	for (auto i = 0; i < levels; i++) {
		hl = subdivide(hl);
	}
	obj->mesh = hl.toMesh();
}
