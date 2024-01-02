#include "06_TextRendering.h"

#include "shared/RenderUtils.h"
#include "shared/Runtime.h"

#include <spw/SimpleWindow.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <imgui.h>


void SPRE_06_TextRendering::OnAttach() {
	// NOTE: In this example, we're using a camera that fits the entire screen, so that all character positions are calculated in screen space
	const glm::vec2 screenOrigin { 0.f, 0.f };
	const glm::vec2 screenSize { spw::getWindowWidth(), spw::getWindowHeight() };
	m_HUDCamera = Camera(
	    glm::vec3(0.f, 0.f, 5.f),
	    glm::vec3(0.f, -90.f, 0.f),
	    glm::vec3(0.f, 1.f, 0.f));
	m_HUDCamera.SetOrthographicRect(screenOrigin, screenSize);

	m_TextPosition = { 100.f, 100.f, 0.f };

	m_QuadModel = Utils::LoadModel("assets/quad_unit.obj");

	m_ModelUniform = spr::createUniform("model", spr::UniformType::Mat4x4);
	m_ViewUniform = spr::createUniform("view", spr::UniformType::Mat4x4);
	m_ProjectionUniform = spr::createUniform("projection", spr::UniformType::Mat4x4);
	m_BaseColorUniform = spr::createUniform("baseColor", spr::UniformType::Vec4);
	m_GlyphUniform = spr::createUniform("glyph", spr::UniformType::Sampler);
	m_DebugUniform = spr::createUniform("debug", spr::UniformType::Integer);
	m_FontShaderProgram = Utils::LoadShaderProgram("shaders/default_vertex.vert", "shaders/font_ttf.frag");


	// 0. Initializes freetype library
	int error = FT_Init_FreeType(&m_FTLibrary);
	assert(error == 0 && "::ERROR::FREETYPE: Could not init FreeType Library");

	// 1. Loads font
	error = FT_New_Face(m_FTLibrary, "assets/fonts/arial.ttf", 0, &m_ArialFontFace);
	assert(error == 0 && "::ERROR::FREETYPE: Could not find font");

	// 2. Sets up the font size for the loaded font.
	// NOTE: Setting width to 0 will force it to fit the same aspect ratio as the height.
	constexpr unsigned int fontWidth = 0;
	const unsigned int fontHeight = std::round(m_FontSize);
	FT_Set_Pixel_Sizes(m_ArialFontFace, fontWidth, fontHeight);

	// TODO: Move to spr!!!
	glClearColor(0.35f, 0.35f, 0.35f, 1.0f);
}

void SPRE_06_TextRendering::OnUpdate() {
	const std::string displayText = "iHello World! go pistons Go Pistons";

	glm::vec3 caretPosition { 0.f };
	for (const char& character : displayText) {
		// 3, 4. Loads character
		if (!LoadCharacter(character)) {
			continue;
		}

		// 5. Calculates character position, considering all the glyph metrics that we loaded from the .ttf font
		const CharacterInstanceTTF &charInstance = m_GlyphCache[character];

		if (character == ' ') {
			caretPosition.x += charInstance.HorizontalSpacing;
			continue;
		}

		const glm::vec3 charAnchor { charInstance.GlyphSize.x * 0.5f, charInstance.GlyphSize.y * 0.5f, 0.f };		// Moves anchor to the center of the quad
		const glm::vec3 charPadding { charInstance.PaddingLeft, -charInstance.PaddingTop, 0.f };					// Padding relative to the character's origin

		const glm::vec3 charPosition = m_TextPosition + caretPosition + charAnchor + charPadding;
		const glm::vec3 charScale { charInstance.GlyphSize.x, charInstance.GlyphSize.y, 1.f };

		glm::mat4 model { 1.0f };
		{
			model = glm::translate(model, charPosition);
			model = glm::rotate(model, 0.f, { 0.f, 1.f, 0.f });
			model = glm::scale(model, charScale * 1.f);
		}

		// 6. Draws one quad for each character
		const auto &quadMesh = m_QuadModel.Meshes[0];
		spr::setVertexBuffer(quadMesh.VertexBuffer);
		spr::setIndexBuffer(quadMesh.IndexBuffer);

		spr::FixedFunctionState fixedFunctionState;
		fixedFunctionState.SetBlendingEnabled(true);
		spr::setFixedFunctionState(fixedFunctionState);

		const int glyphTextureUnit = 0;
		const int bDebugQuads = m_bShouldDebugQuads ? 1 : 0;
		const glm::vec4 baseTextColor { 1.f, 0.f, 1.f, 1.f };
		spr::setTexture(glyphTextureUnit, charInstance.GlyphTextureHandle);
		spr::setUniform(m_GlyphUniform, &glyphTextureUnit);
		spr::setUniform(m_BaseColorUniform, glm::value_ptr(baseTextColor));
		spr::setUniform(m_DebugUniform, &bDebugQuads);

		spr::setUniform(m_ModelUniform, glm::value_ptr(model));
		spr::setUniform(m_ViewUniform, glm::value_ptr(m_HUDCamera.GetView()));
		spr::setUniform(m_ProjectionUniform, glm::value_ptr(m_HUDCamera.GetProjection()));
		spr::submit(0, m_FontShaderProgram);

		caretPosition.x += charInstance.HorizontalSpacing;
	}

	spr::flush();
}

// 3. Loads each character for the string we want to display.
// - Once loaded, you can access it through the FT_Face::glyph::bitmap field
// - Each time a new character is loaded, this field is replaced by the loaded character
// NOTE: We're using the FT_LOAD_RENDER flag here because we intend to render these characters. Check freetype.h to see all flag options.
bool SPRE_06_TextRendering::LoadCharacter(char character) {
	if (m_GlyphCache.count(character) > 0) {
		return true;
	}

	constexpr int charLoadFlags = FT_LOAD_RENDER;
	int error = FT_Load_Char(m_ArialFontFace, character, charLoadFlags);
	assert(error == 0 && "::ERROR: Could not load character");

	// 4. Creates texture on the GPU to store the character's glyph
	const FT_GlyphSlot& characterGlyph = m_ArialFontFace->glyph;
	const FT_Bitmap& characterBitmap = characterGlyph->bitmap;

	// (OPTIONAL) Glyphs only require 1 bit to store color, so we're using the RED format to indicate the renderer to use a single byte per channel
	// NOTE: There is NO glyph for space (' '), it is represented just as an advance in the x-axis.
	spr::TextureInfo glyphTextureInfo;
	glyphTextureInfo.Width = characterBitmap.width;
	glyphTextureInfo.Height = characterBitmap.rows;
	glyphTextureInfo.MipCount = 1;
	glyphTextureInfo.Format = spr::TextureFormat::RED;

	// NOTE: We have to disable byte-alignment when loading images in GL_RED/GL_R8 format, as they only need 1 byte per color
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction, which we need to because we're using a single byte for each glyph texture color
	const glm::vec2 glyphSize { characterBitmap.width, characterBitmap.rows };
	const bool bIsSpaceChar = characterBitmap.width == 0 && characterBitmap.rows == 0;
	m_GlyphCache[character] = {};
	m_GlyphCache[character].GlyphSize = glyphSize;
	m_GlyphCache[character].GlyphTextureHandle = bIsSpaceChar ? spr::k_InvalidHandle : spr::createTexture(glyphTextureInfo, characterBitmap.buffer);
	m_GlyphCache[character].PaddingLeft = characterGlyph->bitmap_left;
	m_GlyphCache[character].PaddingTop = characterGlyph->bitmap_top;
	// Advance is given in 1/64 pixels (i.e., 1 advance unit will move 1/64 pixels to the right), so we're bit shifting by 6 to get the value
	// in pixels, as 2^6 = 64. Alternatively, we could also divide by 64 and would probably get the same result.
	m_GlyphCache[character].HorizontalSpacing = characterGlyph->advance.x >> 6;

	return error == 0;
}

void SPRE_06_TextRendering::OnDetach() {
	Utils::UnloadModel(m_QuadModel);

	spr::destroy(m_FontShaderProgram);
	spr::destroy(m_ModelUniform);
	spr::destroy(m_ViewUniform);
	spr::destroy(m_ProjectionUniform);
	spr::destroy(m_BaseColorUniform);
	spr::destroy(m_GlyphUniform);
	spr::destroy(m_DebugUniform);

	for (auto &[character, characterInstance] : m_GlyphCache) {
		spr::destroy(characterInstance.GlyphTextureHandle);
	}

	// (OPTIONAL) Shuts down freetype library
	if (m_FTLibrary) {
		FT_Done_Face(m_ArialFontFace);
		FT_Done_FreeType(m_FTLibrary);
	}
}


void SPRE_06_TextRendering::OnImGuiRender() {
	ImGui::SetNextWindowSize(ImVec2(300, 500));

	ImGui::Begin("06 - Text");
	ImGui::InputFloat3("Text Position", glm::value_ptr(m_TextPosition));
	ImGui::Checkbox("Debug", &m_bShouldDebugQuads);

	ImGui::End();
}
