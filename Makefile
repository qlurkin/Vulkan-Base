EXE = VulkanLab
CFLAGS = -std=c++17
LDFLAGS = -lvulkan -lglfw -lstdc++
CMD = g++ $(CFLAGS)

OBJECTS = Engine.o Application.o Shader.o VertexInputState.o GraphicsPipeline.o buffers.o VertexBuffer.o IndexBuffer.o CommandBuffer.o UniformBuffer.o Log.o

all: $(EXE) shaders

$(EXE): $(OBJECTS)
	$(CMD) -o $(EXE) $(OBJECTS) $(LDFLAGS)

Log.o: Log.cpp
	$(CMD) -o Log.o -c Log.cpp

Engine.o: Engine.cpp
	$(CMD) -o Engine.o -c Engine.cpp

Application.o: Application.cpp
	$(CMD) -o Application.o -c Application.cpp

Shader.o: Shader.cpp
	$(CMD) -o Shader.o -c Shader.cpp

VertexInputState.o: VertexInputState.cpp
	$(CMD) -o VertexInputState.o -c VertexInputState.cpp

GraphicsPipeline.o: GraphicsPipeline.cpp
	$(CMD) -o GraphicsPipeline.o -c GraphicsPipeline.cpp

buffers.o: buffers.cpp
	$(CMD) -o buffers.o -c buffers.cpp

VertexBuffer.o: VertexBuffer.cpp
	$(CMD) -o VertexBuffer.o -c VertexBuffer.cpp

IndexBuffer.o: IndexBuffer.cpp
	$(CMD) -o IndexBuffer.o -c IndexBuffer.cpp

CommandBuffer.o: CommandBuffer.cpp
	$(CMD) -o CommandBuffer.o -c CommandBuffer.cpp

UniformBuffer.o: UniformBuffer.cpp
	$(CMD) -o UniformBuffer.o -c UniformBuffer.cpp

shaders: shaders/frag.spv shaders/vert.spv

shaders/frag.spv: shaders/shader.frag
	glslangValidator -V -o shaders/frag.spv shaders/shader.frag

shaders/vert.spv: shaders/shader.vert
	glslangValidator -V -o shaders/vert.spv shaders/shader.vert

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXE)
	rm -rf shaders/*.spv