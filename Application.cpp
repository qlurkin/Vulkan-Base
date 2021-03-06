#include "Engine.h"
#include <chrono>

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
};

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class Application : public Engine {
	public:
	void setup() {

		std::vector<Vertex> vertices = {
			{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

			{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
			{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
		};

		std::vector<uint16_t> indices = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		vertexBuffer = new VertexBuffer(vertices.data(), vertices.size(), sizeof(vertices[0]), this);
		indexBuffer = new IndexBuffer(indices.data(), indices.size(), sizeof(indices[0]), this);
		texture = new Texture("textures/texture.jpg", this);

		Shader vertexShader("shaders/vert.spv", this);
		Shader fragmentShader("shaders/frag.spv", this);

		VertexInputState vertexFormat(sizeof(Vertex));
		vertexFormat
			.addAttribute(VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos))
			.addAttribute(VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color))
			.addAttribute(VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, texCoord));

		descriptorSetLayout = new DescriptorSetLayout(this);
		descriptorSetLayout->addUniform().addTexture().end();

		pipeline = new GraphicsPipeline(vertexFormat, vertexShader, fragmentShader, *descriptorSetLayout, this);

		uniformBuffer = new UniformBuffer(sizeof(UniformBufferObject), pipeline);

		descriptorSet = new DescriptorSet(*descriptorSetLayout, this);
		descriptorSet->setUniformBuffer(*uniformBuffer, 0);
		descriptorSet->setTexture(*texture, 1);

		commandBuffer = new CommandBuffer(vertexBuffer, indexBuffer, descriptorSet, pipeline);
	}

	void setupFrame() {
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		
		UniformBufferObject ubo = {};
		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(45.0f), getWidth() / (float) getHeight(), 0.1f, 10.0f);
		ubo.proj[1][1] *= -1;

		uniformBuffer->setData(&ubo);

		add(*commandBuffer);
	}

	void teardown() {
		delete descriptorSet;
		delete descriptorSetLayout;
		delete commandBuffer;
		delete uniformBuffer;
		delete pipeline;
		delete vertexBuffer;
		delete indexBuffer;
		delete texture;
	}

	private:
	GraphicsPipeline* pipeline;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	CommandBuffer* commandBuffer;
	UniformBuffer* uniformBuffer;
	DescriptorSetLayout* descriptorSetLayout;
	DescriptorSet* descriptorSet;
	Texture* texture;
};

int main() {
	Application app;
	return app.run();
}