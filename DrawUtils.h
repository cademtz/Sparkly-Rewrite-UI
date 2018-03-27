#pragma once
#include "Color.h"

typedef unsigned long HFont;
typedef unsigned long HTexture;

enum FontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

#define ESP_HEIGHT 14
class CFonts
{
public:
	void Initialize();
	void Reload();

	HFont arial;
	HFont verdana_bold;
	HFont verdana;
	HFont calibri_light;
	HFont calibri_light_small;
	HFont calibri;
	HFont cambria;

	HFont esp;
};
extern CFonts gFonts;

class CTextures
{
public:
	void Initialize();
	void Reload();

	HTexture dark_gray;
};
extern CTextures gTex;

class TextureHolder
{
public:
	TextureHolder() {}
	TextureHolder(const byte* pRawRGBAData, int W, int H);

	bool IsValid() const
	{
		return m_bValid;
	}

	int GetTextureId() const
	{
		return m_iTexture;
	}

	int GetWidth() const
	{
		return m_iW;
	}

	int GetHeight() const
	{
		return m_iH;
	}

	const byte* GetRGBA() const
	{
		return rawData;
	}

	bool Draw(int x, int y, Color clr = Color(255), float scale = 1.0);

protected:
	int m_iTexture;
	int m_iW, m_iH;
	const byte* rawData;
	bool m_bValid;
};

#include "Vector2.h"
struct Vertex_t
{
	Vertex_t() {}
	Vertex_t(const Vector2 &pos, const Vector2 &coord = Vector2(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}
	void Init(const Vector2 &pos, const Vector2 &coord = Vector2(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}

	Vector2	m_Position;
	Vector2	m_TexCoord;
};