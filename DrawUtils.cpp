#include "DrawUtils.h"
#include "SDK.h"

CFonts gFonts;
CTextures gTex;

//===================================================================================
void CFonts::Initialize()
{
	arial = gInts.Surface->CreateFont();
	verdana_bold = gInts.Surface->CreateFont();
	verdana = gInts.Surface->CreateFont();
	segoe = gInts.Surface->CreateFont();
	calibri_light = gInts.Surface->CreateFont();
	calibri_light_small = gInts.Surface->CreateFont();
	cambria = gInts.Surface->CreateFont();
	esp = gInts.Surface->CreateFont();
}
void CFonts::Reload()
{
	gInts.Surface->SetFontGlyphSet(gFonts.arial, "Arial", ESP_HEIGHT, 500, 0, 0, FONTFLAG_OUTLINE);
	gInts.Surface->SetFontGlyphSet(gFonts.verdana_bold, "Verdana", 14, 800, 0, 0, FONTFLAG_ANTIALIAS);
	gInts.Surface->SetFontGlyphSet(gFonts.verdana, "Verdana", 14, 0, 0, 0, FONTFLAG_ANTIALIAS);
	gInts.Surface->SetFontGlyphSet(gFonts.segoe, "Segoe UI", 24, 250, 0, 0, FONTFLAG_ANTIALIAS);
	gInts.Surface->SetFontGlyphSet(gFonts.calibri_light, "Calibri Light", 24, 250, 0, 0, FONTFLAG_ANTIALIAS);
	gInts.Surface->SetFontGlyphSet(gFonts.calibri_light_small, "Calibri Light", 18, 250, 0, 0, FONTFLAG_ANTIALIAS);
	gInts.Surface->SetFontGlyphSet(gFonts.cambria, "Cambria", 48, 1000, 0, 0, FONTFLAG_ANTIALIAS);
	gInts.Surface->SetFontGlyphSet(gFonts.esp, "Verdana", 14, 800, 0, 0, FONTFLAG_OUTLINE);
}
//===================================================================================
void CTextures::Initialize()
{
	dark_gray = gInts.Surface->CreateNewTextureID();
}
void CTextures::Reload()
{
	static const byte dark_gray_t[4] = { 50, 50, 50, 255 };
	gInts.Surface->DrawSetTextureRGBA(dark_gray, dark_gray_t, 1, 1);
}
//===================================================================================

TextureHolder::TextureHolder(const byte* pRawRGBAData, int W, int H)
{
	m_iTexture = gInts.Surface->CreateNewTextureID(true);
	if (!m_iTexture)
		return;
	gInts.Surface->DrawSetTextureRGBA(m_iTexture, pRawRGBAData, W, H);
	rawData = pRawRGBAData;
	m_iW = W, m_iH = H;
	m_bValid = true;
}
bool TextureHolder::Draw(int x, int y, Color clr, float scale)
{
	if (!gInts.Surface->IsTextureIDValid(m_iTexture))
		return false;
	gInts.Surface->DrawSetColor(clr[0], clr[1], clr[2], clr[3]);
	gInts.Surface->DrawSetTexture(m_iTexture);
	gInts.Surface->DrawTexturedRect(x, y, x + (m_iW * scale), y + (m_iH * scale));
	return true;
}
//===================================================================================
inline void ClampFl(float &fl)
{
	if (fl > 1)
		fl = 1;
	else if (fl < 0)
		fl = 0;
}

Color Color::FromHSB(float Hue, float Saturation, float Brightness, int Alpha)
{
	while (Hue >= 1)
		Hue -= 1;
	while (Hue <= 0)
		Hue += 1;

	ClampFl(Saturation);
	ClampFl(Brightness);

	float h = Hue == 1.0f ? 0 : Hue * 6.0f;
	float f = h - (int)h;
	float p = Brightness * (1.0f - Saturation);
	float q = Brightness * (1.0f - Saturation * f);
	float t = Brightness * (1.0f - (Saturation * (1.0f - f)));

	if (h < 1)
	{
		return Color(
			(unsigned char)(Brightness * 255),
			(unsigned char)(t * 255),
			(unsigned char)(p * 255),
			Alpha
		);
	}
	else if (h < 2)
	{
		return Color(
			(unsigned char)(q * 255),
			(unsigned char)(Brightness * 255),
			(unsigned char)(p * 255),
			Alpha
		);
	}
	else if (h < 3)
	{
		return Color(
			(unsigned char)(p * 255),
			(unsigned char)(Brightness * 255),
			(unsigned char)(t * 255),
			Alpha
		);
	}
	else if (h < 4)
	{
		return Color(
			(unsigned char)(p * 255),
			(unsigned char)(q * 255),
			(unsigned char)(Brightness * 255),
			Alpha
		);
	}
	else if (h < 5)
	{
		return Color(
			(unsigned char)(t * 255),
			(unsigned char)(p * 255),
			(unsigned char)(Brightness * 255),
			Alpha
		);
	}
	else
	{
		return Color(
			(unsigned char)(Brightness * 255),
			(unsigned char)(p * 255),
			(unsigned char)(q * 255),
			Alpha
		);
	}
}