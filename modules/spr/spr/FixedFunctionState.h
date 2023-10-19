#pragma once

#include <cstdint>
#include <cstring>

namespace spr {
	// clang-format off
	struct ColorBufferState {

		enum BlendingFactor : uint8_t {
			FactorZero								= 0b00000,
			FactorOne								= 0b00001,
			FactorSourceColor						= 0b00010,
			FactorOneMinusSourceColor				= 0b00011,
			FactorDestinationColor					= 0b00100,
			FactorOneMinusDestinationColor			= 0b00101,
			FactorSourceAlpha						= 0b00110,
			FactorOneMinusSourceAlpha				= 0b00111,
			FactorDestinationAlpha					= 0b01000,
			FactorOneMinusDestinationAlpha			= 0b01001,
			FactorConstantColor						= 0b01010,
			FactorOneMinusConstantColor				= 0b01011,
			FactorConstantAlpha						= 0b01100,
			FactorOneMinusConstantAlpha				= 0b01101,

			FactorSourceDefault						= FactorSourceAlpha,
			FactorDestinationDefault				= FactorOneMinusSourceAlpha,
		};

		enum BlendingEquation : uint8_t {
			BlendEquationAdd					= 0b000,
			BlendEquationSubtract				= 0b001,
			BlendEquationReverseSubtract		= 0b010,
			BlendEquationMinFn					= 0b011,
			BlendEquationMaxFn					= 0b100,

			BlendEquationDefault				= BlendEquationAdd,
		};

		// byte 1
		uint8_t BlendingSourceFactor			: 5;
		uint8_t RedMask							: 1;
		uint8_t GreenMask						: 1;
		uint8_t BlueMask						: 1;

		// byte 2
		uint8_t AlphaMask						: 1;
		uint8_t bIsBlendingEnabled				: 1;
		uint8_t BlendingDestinationFactor		: 5;
		uint8_t bMSAAEnabled					: 1;

		// byte 3
		uint8_t BlendingEquation				: 3;
		//uint8_t								: 5;


		ColorBufferState() {
			RedMask				= true;
			GreenMask			= true;
			BlueMask			= true;
			AlphaMask			= true;

			bIsBlendingEnabled			= false;
			BlendingSourceFactor		= FactorSourceDefault;
			BlendingDestinationFactor	= FactorDestinationDefault;
			BlendingEquation			= BlendEquationDefault;

			bMSAAEnabled		= true;
		}
	};

	struct DepthBufferState {
		enum DepthTestFn : uint8_t {
			TestFnNever					= 0b000,
			TestFnLess					= 0b001,
			TestFnEqual					= 0b010,
			TestFnLessOrEqual			= 0b011,
			TestFnGreater				= 0b100,
			TestFnNotEqual				= 0b101,
			TestFnGreaterOrEqual		= 0b110,
			TestFnAlways				= 0b111,

			TestFnDefault				= TestFnLess,
		};

		// byte 1
		uint8_t bIsDepthTestEnabled		: 1;
		uint8_t bDepthWriteMask			: 1;
		uint8_t DepthTestFn				: 3;
		//uint8_t						: 3;

		DepthBufferState() {
			bIsDepthTestEnabled = true;
			bDepthWriteMask		= 1;
			DepthTestFn			= DepthTestFn::TestFnDefault;
		}
	};

	struct StencilBufferState {
		enum StencilTestFnType : uint8_t {
			TestFnNever				= 0b000,
			TestFnLess				= 0b001,
			TestFnEqual				= 0b010,
			TestFnLessOrEqual		= 0b011,
			TestFnGreater			= 0b100,
			TestFnNotEqual			= 0b101,
			TestFnGreaterOrEqual	= 0b110,
			TestFnAlways			= 0b111,

			TestFnDefault			= TestFnAlways,
		};

		enum StencilOp {
			OpKeep			= 0b000,
			OpZero			= 0b001,
			OpReplace		= 0b010,
			OpIncr			= 0b011,
			OpIncrWrap		= 0b100,
			OpDecr			= 0b101,
			OpDecrWrap		= 0b110,
			OpInvert		= 0b111,

			OpDefault		= OpKeep, 
		};

		// bytes 1-4
		uint8_t StencilWriteMaskFront;
		uint8_t StencilWriteMaskBack;
		uint8_t StencilTestMask;				// Bitmask applied to both the reference and buffer values before stencil testing
		uint8_t StencilTestReferenceValue;

		// byte 5
		uint8_t StencilTestFn				: 3;
		uint8_t StencilFailOp				: 3;
		uint8_t								: 2;

		// byte 6
		uint8_t StencilPassDepthFailOp		: 3;
		uint8_t StencilPassDepthPassOp		: 3;
		uint8_t bIsStencilTestEnabled		: 1;
		//uint8_t							: 1;

		StencilBufferState() {
			bIsStencilTestEnabled			= false;
			StencilWriteMaskFront			= 0b11111111;
			StencilWriteMaskBack			= 0b11111111;

			StencilTestFn					= StencilTestFnType::TestFnDefault;
			StencilTestReferenceValue		= 0b00000000;
			StencilTestMask					= 0b11111111;

			StencilFailOp					= StencilOp::OpDefault;
			StencilPassDepthFailOp			= StencilOp::OpDefault;
			StencilPassDepthPassOp			= StencilOp::OpDefault;
		}
	};

	// Refer to this for draw call sorting: https://realtimecollisiondetection.net/blog/?p=86
	struct DrawCallSortKey {
		uint8_t bIsBlendingDisabled	: 1;
		uint8_t BlendingEquation	: 3;
		//uint8_t					: 7;

		inline bool operator>(const DrawCallSortKey& otherKey) const {
			// -0: 1st different byte in other is greater than this
			//  0: Equal
			// +0: 1st different byte in this is greater than other
			return memcmp(this, &otherKey, sizeof(DrawCallSortKey)) > 0;
		}

		inline bool operator<(const DrawCallSortKey& otherKey) const {
			return memcmp(this, &otherKey, sizeof(DrawCallSortKey)) < 0;
		}

		inline bool operator==(const DrawCallSortKey& otherKey) const {
			return memcmp(this, &otherKey, sizeof(DrawCallSortKey)) == 0;
		}
	};

	struct FixedFunctionState {
		// There's some bit packing waste here, but we don't care about it
		StencilBufferState	StencilState;
		DepthBufferState	DepthState;
		ColorBufferState	ColorState;

		inline bool operator==(const FixedFunctionState& other) const {
			// Assumes the state is basically a huge bitmask, with only primitive integer types
			return memcmp(this, &other, sizeof(FixedFunctionState)) == 0;
		}

		inline bool operator !=(const FixedFunctionState& other) const {
			return !(*this == other);
		}

		DrawCallSortKey GetSortKey() const {
			DrawCallSortKey sortKey;
			sortKey.bIsBlendingDisabled		= !(!!ColorState.bIsBlendingEnabled);
			sortKey.BlendingEquation		= ColorState.BlendingEquation;
			return sortKey;
		}

		// Color Buffer
		void SetColorWriteEnabled(bool bRedEnabled, bool bGreenEnabled, bool bBlueEnabled, bool bAlphaEnabled) {
			// clang-format off
			ColorState.RedMask		|= bRedEnabled;
			ColorState.GreenMask	|= bGreenEnabled;
			ColorState.BlueMask		|= bBlueEnabled;
			ColorState.AlphaMask	|= bAlphaEnabled;
			// clang-format on
		}

		void SetBlendingEnabled(bool bBlendingEnabled) {
			ColorState.bIsBlendingEnabled = bBlendingEnabled;
		}

		// Stencil Testing
		void SetStencilTestEnabled(bool bEnabled) {
			StencilState.bIsStencilTestEnabled = bEnabled;
		}

		using StencilTestFnType = StencilBufferState::StencilTestFnType;
		void SetStencilTest(StencilTestFnType stencilTestFn, uint8_t refValue, uint8_t testMask) {

			StencilState.StencilTestFn				= stencilTestFn;
			StencilState.StencilTestReferenceValue	= refValue;
			StencilState.StencilTestMask			= testMask;
		}

		using StencilOpType = StencilBufferState::StencilOp;
		void SetStencilOp(StencilOpType stencilFailOp, StencilOpType stencilPassDepthFailOp, StencilOpType bothPassOp) {
			StencilState.StencilFailOp				= stencilFailOp;
			StencilState.StencilPassDepthFailOp		= stencilPassDepthFailOp;
			StencilState.StencilPassDepthPassOp		= bothPassOp;
		}

		void SetStencilWriteMask(uint8_t stencilWriteMask) {
			StencilState.StencilWriteMaskBack		= stencilWriteMask;
			StencilState.StencilWriteMaskFront		= stencilWriteMask;
		}


		// Depth Testing
		void SetDepthTestEnabled(bool bEnabled) {
			DepthState.bIsDepthTestEnabled = bEnabled;
		}

		void SetDepthTestFn(uint8_t depthTestFn) {
			DepthState.DepthTestFn = depthTestFn;
		}
	};

}
