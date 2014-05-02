#ifndef MATERIAL_HPP
#define MATERIAL_HPP

struct Material
{
	Material(const std::string &name, const std::string &desc, char char0, char char1, char char2, char char3, 
		Color bgColor, Color fgColor, bool isBlocking, int image, int flags);
	const std::string name;
	const std::string description;
	~Material();

	Color bgColor;
	Color fgColor;
	int image;
	int ch(const int variantIndex) const{ return c[variantIndex]; }
	const bool isBlocking;
	const int flags;
	static float GetVariantTint(const int variantIndex);
private:
	char c[4];

};

#endif