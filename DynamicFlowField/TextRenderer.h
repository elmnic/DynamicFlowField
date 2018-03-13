#pragma once

#include "Toolbox.h"

class TextRenderer
{
public:
	static TextRenderer* instance();
	~TextRenderer();

	void render();

	void addTextElement(sf::Text* text);

	void clearTextElements();

private:
	TextRenderer();

	std::vector<sf::Text*> mTexts;
};

