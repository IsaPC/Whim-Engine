#include "ElementArrayBuffer.h"
#include <assert.h>

ElementArrayBuffer::ElementArrayBuffer(const unsigned int* data, unsigned int count)
	:	objectId(0),
		count(0)	
{
	assert(sizeof(unsigned int) == sizeof(GLuint));

	glGenBuffers(1, &objectId); // create buffer on gpu
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectId); // use this buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

ElementArrayBuffer::~ElementArrayBuffer()
{
	glDeleteBuffers(1, &objectId);
}

void ElementArrayBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectId); // use this buffer
}

void ElementArrayBuffer::UnBind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // stop using this buffer
}
