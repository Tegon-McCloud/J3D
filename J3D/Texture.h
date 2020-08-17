#pragma once
#include "DXUtils.h"
#include "Bindable.h"

#include <filesystem>

class Texture2D : public Bindable {

public:
	Texture2D(class Graphics& gfx, const std::filesystem::path& file);

	void bind(class Graphics& gfx) override;

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;

};

