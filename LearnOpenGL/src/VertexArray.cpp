#include "VertexArray.h"

VertexArray::VertexArray()
	:	buffer(0)
{
	glGenVertexArrays(1, &buffer);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &buffer);
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(buffer);
}

void VertexArray::UnBind() const
{
	glBindVertexArray(0);
}
