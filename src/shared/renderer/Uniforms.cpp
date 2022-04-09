#include "Uniforms.h"
#include "Handles.h"
#include "Context.h"

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

    using UniformHashMap = std::unordered_map<std::string, HandleType>;

    class UniformRegistry {
	public:
		UniformRegistry()
		{
		}

		~UniformRegistry()
		{
		}

		const UniformHandle* find(const char* name) const
		{
            std::string nameStr { name };
			if (m_uniforms.count(nameStr) > 0)
			{
                HandleType index = m_uniforms.at(nameStr);
				return &m_handles[index];
			}

			return NULL;
		}

		const UniformHandle& add(UniformHandle handle, const char* name)
		{
			assert(handle.isValid() && "::ERROR: Uniform handle is invalid!");

            m_uniforms.erase({ name });
            m_uniforms[{ name }] = handle.idx;

            UniformHandle& outHandle = m_handles[handle.idx];
            outHandle = handle;
			return outHandle;
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
		UniformHandle m_handles[SPR_MAX_UNIFORM_COUNT];
    };

    // Indexed by handles
    static UniformRef s_Uniforms[SPR_MAX_UNIFORM_COUNT];
    
    static UniformHashMap s_UniformHashMap;
    static UniformRegistry s_UniformRegistry;

    // Indexed by uniform location. Data in this array persists through frames.
    static void* s_PersistentUniformData[SPR_MAX_UNIFORM_COUNT];
}

namespace spr {

    static HandleType getAvailableUniformIndex() {
        // Brute-force index generator
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

    static void rendererCreateUniform(UniformHandle handle, const UniformRef& uniformRef) {
        uint32_t size = getUniformSizeByType(uniformRef.Type);
        
        // Initializes empty block of data
        void* data = malloc(size);
        memset(data, 0, size);

        // Registers uniform and its data
        s_PersistentUniformData[handle.idx] = data;
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
        assert(uniformHandle.isValid() && "::ERROR: Invalid uniform handle!");
        const UniformRef& uniform = s_Uniforms[uniformHandle.idx];
        SimpleUniformBufferPtr uniformBuffer = spr::getFrameData().UniformBuffer;

        // Checks whether the uniform buffer needs a resize to fit the new data
        uniformBuffer->update();

        // Writes uniform data to buffer
        uniformBuffer->write(data, getUniformSizeByType(uniform.Type));
    }

    void updateUniform(uint32_t location, const void* data, uint32_t size) {
        memcpy(s_PersistentUniformData[location], data, size);
    }

    void destroyUniform(UniformHandle& uniformHandle) {
    }

}


namespace spr {

    SimpleUniformBuffer::SimpleUniformBuffer(uint32_t size) {
        m_bufferData.reserve(size);
    }
    
    SimpleUniformBufferPtr SimpleUniformBuffer::alloc(uint32_t size) {
        return std::make_shared<SimpleUniformBuffer>(size);
    }
    
    // Updates buffer size when remaining space is smaller than a certain threshold
    void SimpleUniformBuffer::update() {
        static const uint32_t resizeThreshold = 65536;
        static const uint32_t growthPerResize = 1 << 20;

        const uint32_t remainingSpace = m_bufferData.size() - m_writePos;
        if(remainingSpace <= resizeThreshold) {
            m_bufferData.reserve(m_bufferData.size() + growthPerResize);
        }
    }

    void SimpleUniformBuffer::write(const void* data, uint32_t size) {
        bool dataFitsBuffer = m_writePos + size < m_bufferData.size(); 
        assert(dataFitsBuffer && "::ERROR: Buffer size is not big enough for this data");
        
        if(dataFitsBuffer) {
            void* bufferDataWritelocation = m_bufferData.data() + m_writePos;
            memcpy(bufferDataWritelocation, data, size);
            m_writePos += size;
        }
    }

}