#pragma once

#include "Win32Utils.h"

#include <wrl.h>
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <stdint.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace DXUtils {
	
	struct Rational : public DXGI_RATIONAL {
		Rational(
			uint32_t numerator = 0,
			uint32_t denominator = 0
		) :
			DXGI_RATIONAL{ numerator, denominator } {}

	};

	struct ModeDesc : public DXGI_MODE_DESC {
		ModeDesc(
			uint32_t width = 0,
			uint32_t height = 0,
			const Rational& refreshRate = Rational(),
			DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_MODE_SCANLINE_ORDER scanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
			DXGI_MODE_SCALING scaling = DXGI_MODE_SCALING_UNSPECIFIED
		) : 
			DXGI_MODE_DESC{ width, height, refreshRate, format, scanlineOrdering, scaling } {}

	};

	struct SampleDesc : public DXGI_SAMPLE_DESC {
		SampleDesc(
			uint32_t count = 1,
			uint32_t quality = 0
		) :
			DXGI_SAMPLE_DESC{ count, quality } {}
	};

	struct SwapChainDesc : public DXGI_SWAP_CHAIN_DESC {

		SwapChainDesc(
			const ModeDesc& bufferDesc = ModeDesc(),
			const DXUtils::SampleDesc& sampleDesc = DXUtils::SampleDesc(),
			DXGI_USAGE bufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
			uint32_t bufferCount = 2,
			HWND outputWindow = nullptr,
			bool windowed = true,
			DXGI_SWAP_EFFECT swapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
			uint32_t flags = 0
		) :
			DXGI_SWAP_CHAIN_DESC{ bufferDesc, sampleDesc, bufferUsage, bufferCount, outputWindow, windowed, swapEffect, flags } {}

		SwapChainDesc(HWND hWnd) :
			SwapChainDesc() {
			OutputWindow = hWnd;
		}

		SwapChainDesc(Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain) {
			tif(pSwapChain->GetDesc(this));
		}

	};
	
	struct PresentParameters : public DXGI_PRESENT_PARAMETERS {
		PresentParameters(
			uint32_t dirtyRectsCount = 0,
			Win32Utils::Rect* pDirtyRects = nullptr,
			Win32Utils::Rect* pScrollRect = nullptr,
			Win32Utils::Point* pScrollOffset = nullptr
		) :
			DXGI_PRESENT_PARAMETERS{ dirtyRectsCount, pDirtyRects, pScrollRect, pScrollOffset } {}

	};

	struct Viewport : public D3D11_VIEWPORT {
		Viewport(
			float topLeftX = 0.0f,
			float topLeftY = 0.0f,
			float width = 0.0f,
			float height = 0.0f,
			float minDepth = D3D11_MIN_DEPTH,
			float maxDepth = D3D11_MAX_DEPTH
		) :
			D3D11_VIEWPORT{ topLeftX, topLeftY, width, height, minDepth, maxDepth } {}

		Viewport(float width, float height) : Viewport() {	
			Width = width;
			Height = height;
		}

		Viewport(const SwapChainDesc& swapChainDesc) :
			Viewport() {
			Width = static_cast<float>(swapChainDesc.BufferDesc.Width);
			Height = static_cast<float>(swapChainDesc.BufferDesc.Height);
		}	
	};

	struct Tex2DRTV : public D3D11_TEX2D_RTV {
		Tex2DRTV(
			uint32_t mipSlice = 0
		) :
			D3D11_TEX2D_RTV{ mipSlice } {}

	};

	struct RenderTargetViewDesc : public D3D11_RENDER_TARGET_VIEW_DESC {
		RenderTargetViewDesc(
			DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_RTV_DIMENSION viewDimension = D3D11_RTV_DIMENSION_TEXTURE2D,
			const Tex2DRTV& texture2D = Tex2DRTV()
		) :
			D3D11_RENDER_TARGET_VIEW_DESC{ format, viewDimension } {
			Texture2D = texture2D;
		}
	};

	struct BufferDesc : public D3D11_BUFFER_DESC {
		BufferDesc(
			uint32_t byteWidth,
			D3D11_USAGE usage,
			uint32_t bindFlags,
			uint32_t cpuAccessFlags,
			uint32_t miscFlags,
			uint32_t structureByteStride
		) : D3D11_BUFFER_DESC{ byteWidth, usage, bindFlags, cpuAccessFlags, miscFlags, structureByteStride } {}
	};

	struct SubresourceData : public D3D11_SUBRESOURCE_DATA {
		SubresourceData(
			const void* pSysMem,
			uint32_t sysMemPitch,
			uint32_t sysMemSlice
		) :
			D3D11_SUBRESOURCE_DATA{ pSysMem, sysMemPitch, sysMemSlice } {}

	};

}
