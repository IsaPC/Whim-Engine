#pragma once
#include <glad/glad.h>

class ElementArrayBuffer
{
private:
	unsigned int objectId;
	unsigned int count;
public:
	ElementArrayBuffer(const unsigned int* data, unsigned int count);
	~ElementArrayBuffer();

	void Bind() const;
	void UnBind();

	inline unsigned int GetCount() { return count; }
};

