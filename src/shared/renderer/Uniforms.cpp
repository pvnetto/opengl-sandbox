#include "Uniforms.h"
#include "Handles.h"
#include "Context.h"

#include <unordered_map>
#include <string>
#include <cstring>
#include <cassert>
#include <glad/glad.h>

// ========================================
// ==== Uniforms ==========================
// ========================================
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

			return nullptr;
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
		UniformHandle m_handles[UniformHandle::capacity];
    };

    // Indexed by handles
    static UniformRef s_Uniforms[UniformHandle::capacity];
    
    static UniformHashMap s_UniformHashMap;
    static UniformRegistry s_UniformRegistry;

    // Data in this array persists through frames, so constant uniforms can be set only once.
    static void* s_PersistentUniformData[UniformHandle::capacity];
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
        case UniformType::Sampler:
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
        UniformHandle handle;

        // If index exists in hashmap, just check uniform validity and return a handle
        if(s_UniformHashMap.count({ name }) > 0) {
            handle.idx = s_UniformHashMap[name];
            uniformRef = &s_Uniforms[handle.idx];
            assert(uniformRef->Type == type && "::ERROR: Uniform type mismatch");
        }
        else {
            // Generates new uniform ref
            handle = HandleGenerator<UniformHandle>::allocHandle();
            uniformRef = &s_Uniforms[handle.idx];
            uniformRef->Name = name;
            uniformRef->Type = type;

            // Adds ref index to hashmap
            s_UniformHashMap[{ name }] = handle.idx;
        }

        rendererCreateUniform(handle, *uniformRef);
        return handle;
    }

    void setUniform(const UniformHandle& uniformHandle, const void* data) {
        assert(uniformHandle.isValid() && "::ERROR: Invalid uniform handle!");
        const UniformRef& uniform = s_Uniforms[uniformHandle.idx];
        const FrameData& frameData = spr::getFrameData();
        const UniformDataBufferPtr& uniformDataBuffer = frameData.UniformDataBuffer;

        // Writes uniform data to buffer
        uniformDataBuffer->write(&uniformHandle, sizeof(UniformHandle));
        uniformDataBuffer->write(&uniform.Type, sizeof(UniformType));
        uniformDataBuffer->write(data, getUniformSizeByType(uniform.Type));
    }

    void destroy(UniformHandle& uniformHandle) {
        // TODO: Free uniform data from memory
        s_UniformRegistry.remove(uniformHandle);
        HandleGenerator<UniformHandle>::removeHandle(uniformHandle);
    }

    static void updateUniform(const UniformHandle& handle, const void* data, uint32_t size) {
        memcpy(s_PersistentUniformData[handle.idx], data, size);
    }

    void updateUniforms(UniformDataBufferPtr uniformDataBuffer, uint32_t start, uint32_t end)
    {
        uniformDataBuffer->reset(start);

        while(uniformDataBuffer->getPos() != end) {
            UniformHandle handle = uniformDataBuffer->read<UniformHandle>();
            UniformType type = uniformDataBuffer->read<UniformType>();

            const uint32_t uniformSize = getUniformSizeByType(type);
            void* data = uniformDataBuffer->read(uniformSize);
            updateUniform(handle, data, uniformSize);
        }
    }

    template <typename T>
    T* getPersistentUniformData(const UniformHandle& handle) {
        return static_cast<T*>(s_PersistentUniformData[handle.idx]);
    }

    void rendererSetUniforms(UniformInfoBufferPtr uniformInfoBuffer) {
        // Assumes buffer is at end location
        // TODO: Read all uniforms at once so we don't have to rely on the buffer being at the right state
        uint32_t bufferEnd = uniformInfoBuffer->getPos();
        uniformInfoBuffer->reset();
        while(uniformInfoBuffer->getPos() < bufferEnd) {
            UniformType type = uniformInfoBuffer->read<UniformType>();
            uint32_t location = uniformInfoBuffer->read<uint32_t>();
            UniformHandle handle = uniformInfoBuffer->read<UniformHandle>();
            switch(type) {
                case UniformType::Float:
                    glUniform1fv(location, 1, getPersistentUniformData<float>(handle));
                    break;
                case UniformType::Sampler:
                case UniformType::Integer:
                    glUniform1iv(location, 1, getPersistentUniformData<int>(handle));
                    break;
                case UniformType::Vec2:
                    glUniform2fv(location, 1, getPersistentUniformData<float>(handle));
                    break;
                case UniformType::Vec3:
                    glUniform3fv(location, 1, getPersistentUniformData<float>(handle));
                    break;
                default:
                    assert(false && "::ERROR: Undefined uniform type");
                    break;
            }
        }
    }

    UniformHandle getUniformByName(const char* name) {
        if(const UniformHandle* handle = s_UniformRegistry.find(name)) {
            return *handle;
        }
        assert(false && "::ERROR: Uniform doesn't exist. You should call spr::createUniform before calling this.");
        return { kInvalidHandle };
    }

}
