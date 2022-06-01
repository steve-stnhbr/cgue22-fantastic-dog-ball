 #include "Render.h"

#include <GLFW/glfw3.h>
#include <glm/ext/scalar_constants.hpp>
#include <assimp/Importer.hpp>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags

Render::Mesh::Mesh(std::vector<Vertex> v_, std::vector<unsigned> i_) : vertex_array(std::move(v_)), index_array(
	                                                                       std::move(i_))
{
}

void Render::Mesh::createBuffers(unsigned int vaoID) const
{
	glCreateBuffers(1, (GLuint*)&vboID);
	auto vboSize = vertex_array.size() * sizeof(Vertex);
	Loggger::trace("Creating vbo with size of %u", vboSize);
	glNamedBufferStorage(vboID, vboSize, vertex_array.data(), GL_CLIENT_STORAGE_BIT);
	glVertexArrayVertexBuffer(vaoID, 0, vboID, 0, sizeof(Vertex));
	Utils::checkError();

	glCreateBuffers(1, (GLuint*)&eboID);
	auto eboSize = index_array.size() * sizeof(unsigned);
	Loggger::trace("Creating vbo with size of %u", eboSize);
	glNamedBufferStorage(eboID, eboSize, index_array.data(), GL_CLIENT_STORAGE_BIT);
	glVertexArrayElementBuffer(vaoID, eboID);
	Utils::checkError();
}

void Render::Mesh::bind(Shaders::Program prog)
{
	glBindVertexBuffer(0, vboID, 0, sizeof(Vertex));
	Utils::checkError();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	Utils::checkError();
}

std::vector<Render::Mesh> Render::Mesh::fromFile(const std::string& path)
{
	return fromFile(path, aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords);
}

std::vector<Render::Mesh> Render::Mesh::fromFile(const std::string& path, const unsigned flags)
{
	std::vector<Mesh> meshes;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path.c_str(), flags);
  
	if (scene == nullptr) {
		Loggger::error(importer.GetErrorString());
		meshes.push_back({});
		return meshes;
	}

	for(unsigned i = 0; i < scene->mNumMeshes; i++)
	{
		Mesh m;
		m.fileName = path;
		const aiMesh* mesh = scene->mMeshes[i];
		for (unsigned j = 0; j < mesh->mNumVertices; ++j)
		{
			const aiVector3D v = mesh->mVertices[j];
			const aiVector3D n = mesh->HasNormals() ? mesh->mNormals[j] : aiVector3D {0};
			const aiVector3D t = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][j] : aiVector3D{ 0 };

			m.vertex_array.push_back({
				{v.x, v.y, v.z},
				{n.x, n.y, n.z},
				{t.x, t.y}
			});
		}

		for (unsigned j = 0; j < mesh->mNumFaces; ++j)
		{
			const aiFace face = mesh->mFaces[j];
			for (unsigned k = 0; k < face.mNumIndices; ++k)
			{
				m.index_array.push_back(face.mIndices[k]);
			}
		}

		meshes.push_back(m);
	}

	return meshes;
}

Render::Cube::Cube(float centerX, float centerY, float centerZ, float width, float height, float depth) : Mesh({
	                                                                                                               // front
	                                                                                                               Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, -height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(0, 0, 1),
		                                                                                                               {1,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, -height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(0, 0, 1),
		                                                                                                               {0,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(0, 0, 1),
		                                                                                                               {0,1},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(0, 0, 1),
		                                                                                                               {1,1},
	                                                                                                               },
	                                                                                                               // back
	                                                                                                               Vertex {
		                                                                                                               glm::vec3(width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(0, 0, -1),
		                                                                                                               {1,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(0, 0, -1),
		                                                                                                               {0,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(0, 0, -1),
		                                                                                                               {0,1},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(0, 0, -1),
		                                                                                                               {1,1},
	                                                                                                               },
	                                                                                                               //right
	                                                                                                               Vertex {
		                                                                                                               glm::vec3(width / 2.0f, -height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(1, 0, 0),
		                                                                                                               {1,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(1, 0, 0),
		                                                                                                               {0,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(1, 0, 0),
		                                                                                                               {0,1},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(1, 0, 0),
		                                                                                                               {1,1},
	                                                                                                               },
	                                                                                                               //left
	                                                                                                               Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(-1, 0, 0),
		                                                                                                               {1,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, -height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(-1, 0, 0),
		                                                                                                               {0,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(-1, 0, 0),
		                                                                                                               {0,1},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(-1, 0, 0),
		                                                                                                               {1,1},
	                                                                                                               },
	                                                                                                               //top
	                                                                                                               Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(0, 1, 0),
		                                                                                                               {1,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(0, 1, 0),
		                                                                                                               {0,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(0, 1, 0),
		                                                                                                               {0,1},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(0, 1, 0),
		                                                                                                               {1,1},
	                                                                                                               },
	                                                                                                               //bottom
	                                                                                                               Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(0, -1, 0),
		                                                                                                               {1,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		                                                                                                               glm::vec3(0, -1, 0),
		                                                                                                               {0,0},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(width / 2.0f, -height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(0, -1, 0),
		                                                                                                               {0,1},
	                                                                                                               },Vertex {
		                                                                                                               glm::vec3(-width / 2.0f, -height / 2.0f,  depth / 2.0f),
		                                                                                                               glm::vec3(0, -1, 0),
		                                                                                                               {1,1},
	                                                                                                               },
                                                                                                               }, {
	                                                                                                               // front
	                                                                                                               0, 1, 2,
	                                                                                                               2, 3, 0,
	                                                                                                               // back
	                                                                                                               4, 5, 6,
	                                                                                                               6, 7, 4,
	                                                                                                               // right
	                                                                                                               8, 9, 10,
	                                                                                                               10, 11, 8,
	                                                                                                               // left
	                                                                                                               12, 13, 14,
	                                                                                                               14, 15, 12,
	                                                                                                               // top
	                                                                                                               16, 17, 18,
	                                                                                                               18, 19, 16,
	                                                                                                               // bottom
	                                                                                                               20, 21, 22,
	                                                                                                               22, 23, 20
                                                                                                               })
{
}

Render::Sphere::Sphere(float radius, unsigned longitude_segments, unsigned latitude_segments) : Mesh()
{
	std::vector<Vertex> vertecies;
	std::vector<unsigned> indices;

	vertecies.push_back({
		glm::vec3(0.0f, radius, 0.0f),
		glm::vec3(0.0f, radius, 0.0f),
		glm::vec2(0, 0)
		});
	vertecies.push_back({
			glm::vec3(0.0f, -radius, 0.0f),
			glm::vec3(0.0f, -radius, 0.0f),
			glm::vec2(1, 1)
	});

	// first and last ring
	for (uint32_t j = 0; j < longitude_segments; j++) {
		indices.push_back(0);
		indices.push_back(j == longitude_segments - 1 ? 2 : (j + 3));
		indices.push_back(2 + j);

		indices.push_back(2 + (latitude_segments - 2) * longitude_segments + j);
		indices.push_back(j == longitude_segments - 1 ? 2 + (latitude_segments - 2) * longitude_segments : 2 + (latitude_segments - 2) * longitude_segments + j + 1);
		indices.push_back(1);
	}

	// vertices and rings
	for (uint32_t i = 1; i < latitude_segments; i++) {
		const float verticalAngle = float(i) * glm::pi<float>() / float(latitude_segments);
		for (uint32_t j = 0; j < longitude_segments; j++) {
			const float horizontalAngle = float(j) * 2.0f * glm::pi<float>() / float(longitude_segments);
			glm::vec3 position = glm::vec3(
				radius * glm::sin(verticalAngle) * glm::cos(horizontalAngle),
				radius * glm::cos(verticalAngle),
				radius * glm::sin(verticalAngle) * glm::sin(horizontalAngle)
			);
			auto u = (0.5 + atan2(position.x, position.z) / (2 * glm::pi<double>()));
			auto v = (0.5 - asin(position.y) / glm::pi<double>());

			vertecies.push_back({
				position,
				glm::normalize(position),
				{u, v}
				});
			

			if (i == 1) continue;

			indices.push_back(2 + (i - 1) * longitude_segments + j);
			indices.push_back(j == longitude_segments - 1 ? 2 + (i - 2) * longitude_segments : 2 + (i - 2) * longitude_segments + j + 1);
			indices.push_back(j == longitude_segments - 1 ? 2 + (i - 1) * longitude_segments : 2 + (i - 1) * longitude_segments + j + 1);

			indices.push_back(j == longitude_segments - 1 ? 2 + (i - 2) * longitude_segments : 2 + (i - 2) * longitude_segments + j + 1);
			indices.push_back(2 + (i - 1) * longitude_segments + j);
			indices.push_back(2 + (i - 2) * longitude_segments + j);
		}
	}

	vertex_array = vertecies;
	index_array = indices;
}


unsigned Render::loadProgram(const std::vector<GLenum>& types, const std::vector<std::string>& files)
{
	try
	{
		const unsigned program = Shaders::loadShadersFile(types, files);
		return program;
	}
	catch (std::length_error& e)
	{
		Loggger::error("Impossible");
		glfwTerminate();
		exit(-11);
	}
	catch (Shaders::ShaderCompilationException& e)
	{
		Loggger::error("Shader compile error (%s):%s\n", e.shaderName.c_str(), e.what());
		glfwTerminate();
		exit(-12);
	}
	catch (Shaders::ProgramLinkException& e)
	{
		Utils::checkError();
		Loggger::error("Program link error (%d):%s\n", e.program, e.what());
		glfwTerminate();
		exit(-13);
	}
}

Render::Plane::Plane(float width, float depth) : Mesh({
		{
			{width / 2, 0, depth / 2},
			{0, 1, 0},
			{1, 1}
		},{
			{width / 2, 0, -depth / 2},
			{0, 1, 0},
			{1, 0}
		},{
			{-width / 2, 0, -depth / 2},
			{0, 1, 0},
			{0, 0}
		},{
			{-width / 2, 0, depth / 2},
			{0, 1, 0},
			{0, 1}
		},
	}, {0,1,2,0,2,3})
{
}
