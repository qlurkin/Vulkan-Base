#include "VertexInputState.h"
#include "Log.h"

VertexInputState::VertexInputState(size_t stride) {
	bindingDescription = {};
	bindingDescription.binding = 0;
	bindingDescription.stride = stride;
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

	LOG << "VertexInputState Created" << ENDL;
}

VertexInputState::~VertexInputState() {
	LOG << "VertexInputState Destroyed" << ENDL;
}

VertexInputState::operator VkPipelineVertexInputStateCreateInfo&() {
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	return vertexInputInfo;
}

VertexInputState& VertexInputState::addAttribute(VkFormat format, uint32_t offset) {
	VkVertexInputAttributeDescription attributeDescription = {};

	attributeDescription.binding = 0;
	attributeDescription.location = attributeDescriptions.size();
	attributeDescription.format = format;
	attributeDescription.offset = offset;

	attributeDescriptions.push_back(attributeDescription);

	return *this;
}

