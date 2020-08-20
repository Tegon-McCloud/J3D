#pragma once

class Graphics;

#include "DXUtils.h"
#include "Bindable.h"

#include <filesystem>

class Texture2D {
public:
	Texture2D(Graphics& gfx, const std::filesystem::path& file);
	
protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
};

class ShaderTexture2D : public Texture2D, public Bindable {
public:
	ShaderTexture2D(Graphics& gfx, const std::filesystem::path& file, uint8_t slot = 0);
	
	virtual void bind(Graphics& gfx, uint8_t slot) = 0;

protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pView;
	uint8_t slot;
};

class VSTexture2D : public ShaderTexture2D {
public:
	using ShaderTexture2D::ShaderTexture2D;

	void bind(Graphics& gfx) override;
	void bind(Graphics& gfx, uint8_t slot) override;
};

class PSTexture2D : public ShaderTexture2D {
public:
	using ShaderTexture2D::ShaderTexture2D;

	void bind(class Graphics& gfx) override;
	void bind(Graphics& gfx, uint8_t slot) override;

};

