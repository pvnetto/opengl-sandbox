#pragma once

#include <cstdint>
#include <cstring>

namespace spr {
	// clang-format off
	struct ColorBufferState {
		uint8_t bColorWriteMask			: 1;
		//uint8_t						: 7;

		ColorBufferState() {
			bColorWriteMask = 1;
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
			bIsStencilTestEnabled				= false;
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


	struct FixedFunctionState {
		DepthBufferState	DepthState;			// head: -,		tail: -3b
		StencilBufferState	StencilState;		// head: +8b,	tail: -1b
		ColorBufferState	ColorState;			// head: +1b,	tail: -7b

		inline bool operator==(const FixedFunctionState& other) const {
			// Assumes the state is basically a huge bitmask, with only primitive integer types
			return memcmp(this, &other, sizeof(FixedFunctionState)) == 0;
		}

		inline bool operator !=(const FixedFunctionState& other) const {
			return !(*this == other);
		}

		// Color Buffer
		void SetColorWriteEnabled(bool bEnabled) {
			ColorState.bColorWriteMask = bEnabled;
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
