#include "main.hpp"

Material::Material(const std::string &name, const std::string &desc, char char0, char char1, char char2, char char3, 
Color bgColor, Color fgColor, bool isBlocking, int image, int flags):
name(name), description(desc), bgColor(bgColor), fgColor(fgColor), isBlocking(isBlocking), image(image), flags(flags)
{
	c[0] = char0;
	c[1] = char1;
	c[2] = char2;
	c[3] = char3;
}

float Material::GetVariantTint(const int variantIndex)
{
	float tint = 1.0f;
	switch(variantIndex)
	{
	case 0: tint = 1.2; break;
	case 1: tint = 1.0; break;
	case 2: tint = 1.2; break;
	case 3: tint = 1.3; break;
	default: tint = 1.0; break;
	}

	return tint;
}

Material::~Material()
{}