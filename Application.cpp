#include "Engine.h"

struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;
};

class Application : public Engine {
	public:
	void setup() {

		std::vector<Vertex> vertices = {
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};

		std::vector<uint16_t> indices = {
			0, 1, 2, 2, 3, 0
		};

		struct UniformBufferObject {
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 proj;
		};

		vertexBuffer = new VertexBuffer(vertices.data(), vertices.size(), sizeof(vertices[0]), this);
		indexBuffer = new IndexBuffer(indices.data(), indices.size(), sizeof(indices[0]), this);
	}

	void initPipelines() {
		Shader vertexShader("shaders/vert.spv", this);
		Shader fragmentShader("shaders/frag.spv", this);

		VertexInputState vertexFormat(sizeof(Vertex));
		vertexFormat
			.addAttribute(VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, pos))
			.addAttribute(VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color));

		pipeline = new GraphicsPipeline(vertexFormat, vertexShader, fragmentShader, this);

		commandBuffer = new CommandBuffer(vertexBuffer, indexBuffer, pipeline);
	}

	void setupFrame() {
		add(commandBuffer);
	}

	void cleanupPipelines() {
		delete commandBuffer;
		delete pipeline;
	}

	void teardown() {
		delete vertexBuffer;
		delete indexBuffer;
	}

	private:
	GraphicsPipeline* pipeline;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	CommandBuffer* commandBuffer;
};

int main() {
	Application app;
	return app.run();
}