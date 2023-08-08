#pragma once

// TODO: Remove all of these one by one by refactoring them into components that make sense
#include "Context.h"
#include "Handles.h"
#include "Runtime.h"
#include "Inputs.h"
#include "Programs.h"
#include "VertexBuffer.h"
#include "VertexAttributeLayout.h"

// TODO: Once everything is refactored, remove this!!
namespace spr {

	Context *getContext();

}

namespace spr {
	struct ProgramHandle;
	struct IndexBufferHandle;
}


namespace spr {

	void init();
	void shutdown();

	void submit(ProgramHandle &program);
	void clean();
	
	void clear();
	void render();

	void setVertexBuffer(const VertexBufferHandle &handle);
	void setIndexBuffer(const IndexBufferHandle &handle);
	void setUniform(const UniformHandle &uniformHandle, const void *data);

    IndexBufferHandle createIndexBuffer(const void *data, uint32_t size);
	void destroy(IndexBufferHandle &handle);

	UniformHandle createUniform(const char *name, UniformType type);
	void destroy(UniformHandle &uniformHandle);

}