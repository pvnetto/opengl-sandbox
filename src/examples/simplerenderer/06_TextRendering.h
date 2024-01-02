#pragma once

#include "shared/Camera.h"
#include "shared/RenderUtils.h"
#include "shared/layers/Layer.h"
#include <spr/SimpleRenderer.h>

#include <ft2build.h>
#include FT_FREETYPE_H

struct CharacterInstanceTTF {
	glm::ivec2 GlyphSize;
	float PaddingLeft;						// distance from the origin to the start of the glyph texture
	float PaddingTop;						// distance from the origin to the top of the glyph texture
	float HorizontalSpacing;				// distance from the origin to the next glyph
	spr::TextureHandle GlyphTextureHandle;
};

class SPRE_06_TextRendering : public Layer {
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;

private:
	bool LoadCharacter(char character);

private:
	Camera m_HUDCamera;
	float m_FontSize = 64.f;
	bool m_bShouldDebugQuads = false;
	glm::vec3 m_TextPosition;

private:
	Utils::ModelInstance m_QuadModel;

	spr::ProgramHandle m_FontShaderProgram;
	spr::UniformHandle m_ModelUniform;
	spr::UniformHandle m_ViewUniform;
	spr::UniformHandle m_ProjectionUniform;
	spr::UniformHandle m_BaseColorUniform;
	spr::UniformHandle m_GlyphUniform;
	spr::UniformHandle m_DebugUniform;

private:
	FT_Library m_FTLibrary;
	FT_Face m_ArialFontFace;

	std::unordered_map<char, CharacterInstanceTTF> m_GlyphCache;
};
