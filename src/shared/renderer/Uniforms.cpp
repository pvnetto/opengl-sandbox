#include "Uniforms.h"
#include "Handles.h"

#include <unordered_map>
#include <string>
#include <cassert>

// ========================================
// ==== Uniforms ==========================
// ========================================

namespace spr {

    #define SPR_MAX_UNIFORM_COUNT 512

}


namespace spr {

    struct UniformRef {
        std::string Name;
        UniformType Type;
    };

    struct UniformRegInfo {
        UniformHandle Handle;
    };

    using UniformHashMap = std::unordered_map<std::string, HandleType>;

    class UniformRegistry {
	public:
		UniformRegistry()
		{
		}

		~UniformRegistry()
		{
		}

		const UniformRegInfo* find(const char* name) const
		{
            std::string nameStr { name };
			if (m_uniforms.count(nameStr) > 0)
			{
                HandleType index = m_uniforms.at(nameStr);
				return &m_info[index];
			}

			return NULL;
		}

		const UniformRegInfo& add(UniformHandle handle, const char* name)
		{
			assert(handle.isValid() && "::ERROR: Uniform handle is invalid!");

            m_uniforms.erase({ name });
            m_uniforms[{ name }] = handle.idx;

			UniformRegInfo& info = m_info[handle.idx];
			info.Handle = handle;

			return info;
		}

		void remove(UniformHandle handle)
		{
            for(const auto& pair : m_uniforms) {
                if(pair.second == handle.idx) {
                    m_uniforms.erase(pair.first);
                    break;
                }
            }
		}

	private:
		UniformHashMap m_uniforms;
		UniformRegInfo m_info[SPR_MAX_UNIFORM_COUNT];
    };


    /* Buffer data container. */
    class SimpleUniformBuffer {
    public:
        static SimpleUniformBuffer* alloc(uint32_t size) {
            SimpleUniformBuffer* newUniformBuffer = new SimpleUniformBuffer();
            newUniformBuffer->m_bufferData.reserve(size);
            return newUniformBuffer;
        }

        static void destroy(SimpleUniformBuffer* buffer) {
            buffer->m_bufferData.clear();
            buffer->~SimpleUniformBuffer();
        }

        static void update(SimpleUniformBuffer* buffer) {

        }

    private:
        std::vector<char> m_bufferData;
    };

    static UniformRef s_Uniforms[SPR_MAX_UNIFORM_COUNT];
    static UniformHashMap s_UniformHashMap;
    static UniformRegistry s_UniformRegistry;

    static void* s_UniformData[SPR_MAX_UNIFORM_COUNT];
}

namespace spr {

    // Brute-force index generator
    static HandleType getAvailableUniformIndex() {
        for(int i = 0; i < SPR_MAX_UNIFORM_COUNT; i++) {
            if(s_Uniforms[i].Name == "") {
                return i;
            }
        }

        return kInvalidHandle;
    }

    static uint32_t getUniformSizeByType(UniformType type) {
        switch(type) {
        case UniformType::Float:
            return sizeof(float);
        case UniformType::Integer:
            return sizeof(int);
        case UniformType::Vec2:
            return sizeof(float) * 2;
        case UniformType::Vec3:
            return sizeof(float) * 3;
        case UniformType::Vec4:
            return sizeof(float) * 4;
        default:
            assert(false && "::ERROR: Undefined size for specified uniform type");
            return 0;
        }
    }

    static void* allocData(uint32_t size) {
        char* dataBuffer = new char[size];
        return static_cast<void*>(dataBuffer);
    }

    static void rendererCreateUniform(UniformHandle handle, const UniformRef& uniformRef) {
        uint32_t size = getUniformSizeByType(uniformRef.Type);
        void* data = allocData(size);
        s_UniformData[handle.idx] = data;
        s_UniformRegistry.add(handle, uniformRef.Name.c_str());
    }

    UniformHandle createUniform(const char* name, UniformType type) {
        HandleType index = kInvalidHandle;
        UniformRef* uniformRef = nullptr;

        // If index exists in hashmap, just check uniform validity and return a handle
        if(s_UniformHashMap.count({ name }) > 0) {
            index = s_UniformHashMap[name];
            uniformRef = &s_Uniforms[index];
            assert(uniformRef->Type == type && "::ERROR: Uniform type mismatch");
        }
        else {
            // Generates new uniform ref
            index = getAvailableUniformIndex();
            uniformRef = &s_Uniforms[index];
            uniformRef->Name = name;
            uniformRef->Type = type;

            // Adds ref index to hashmap
            s_UniformHashMap[{ name }] = index;
        }

        UniformHandle handle { index };
        rendererCreateUniform(handle, *uniformRef);
        return handle;
    }

    void setUniform(const UniformHandle& uniformHandle, const void* data) {
        assert(uniformHandle.isValid(), "::ERROR: Invalid uniform handle!");
        const UniformRef& uniform = s_Uniforms[uniformHandle.idx];

        // TODO: Update uniform buffer
        // TODO: Write value to uniform buffer

        // UniformBuffer::update(&m_frame->m_uniformBuffer[m_uniformIdx]);
        // UniformBuffer* uniformBuffer = m_frame->m_uniformBuffer[m_uniformIdx];
        // uniformBuffer->writeUniform(_type, _handle.idx, _value, _num);
    }

    void destroy(UniformHandle& uniformHandle) {

    }


}