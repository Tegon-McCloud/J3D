#include "Texture.h"

#include "Graphics.h"

#include <wincodec.h>

#include <string>
#include <cassert>
#include <vector>

using Microsoft::WRL::ComPtr;

Texture2D::Texture2D(Graphics& gfx, const std::filesystem::path& file) {
	ComPtr<IWICImagingFactory2> pImgFac;
	tif(CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, __uuidof(pImgFac), &pImgFac));

	ComPtr<IWICBitmapDecoder> pDecoder;
	tif(pImgFac->CreateDecoderFromFilename(file.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder));
	
	ComPtr<IWICBitmapFrameDecode> pDecode;
	tif(pDecoder->GetFrame(0, &pDecode));

	//WICPixelFormatGUID format;
	//tif(pDecode->GetPixelFormat(&format));
	
	//ComPtr<IWICComponentInfo> pCompInfo;
	//tif(pImgFac->CreateComponentInfo(format, &pCompInfo));
	
	//ComPtr<IWICPixelFormatInfo2> pFormatInfo;
	//tif(pCompInfo.As(&pFormatInfo));

	//std::wstring name;
	//name.resize(256);
	//UINT actualSize;
	//pCompInfo->GetFriendlyName(name.length(), name.data(), &actualSize);
	
	ComPtr<IWICFormatConverter> pConverter;
	tif(pImgFac->CreateFormatConverter(&pConverter));

	//ComPtr<IWICPalette> pPalette;
	//tif(pImgFac->CreatePalette(&pPalette));
	//tif(pPalette->InitializeFromBitmap(pDecode.Get(), 0, true));

	tif(pConverter->Initialize(pDecode.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom));

	

	D3D11_TEXTURE2D_DESC desc;
	tif(pConverter->GetSize(&desc.Width, &desc.Height));
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	std::vector<std::byte> imgData(4 * desc.Width * desc.Height);
	pConverter->CopyPixels(nullptr, 4, imgData.size(), reinterpret_cast<BYTE*>(imgData.data()));

	D3D11_SUBRESOURCE_DATA initDesc;
	initDesc.pSysMem = imgData.data();
	initDesc.SysMemPitch = desc.Width * 4;
	initDesc.SysMemSlicePitch = desc.Width * desc.Height * 4;

	tif(gfx.getDevice().CreateTexture2D(&desc, &initDesc, &pTexture));

}

void Texture2D::bind(Graphics& gfx) {}