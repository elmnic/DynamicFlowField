#include "TextRenderer.h"
#include "Toolbox.h"

#include <iostream>

TextRenderer::TextRenderer()
{
}


TextRenderer* TextRenderer::instance()
{
	static TextRenderer instance;
	return &instance;
}

TextRenderer::~TextRenderer()
{
	clearTextElements();
}

void TextRenderer::render()
{
	// Position each text above each other
	for (size_t i = 0; i < mTexts.size(); i++)
	{
		Toolbox::getWindow().draw(*mTexts[i]);
	}
}

void TextRenderer::addTextElement(sf::Text* text)
{
	// Add text and move it down
	text->setCharacterSize(35);
	text->setFillColor(sf::Color::Black);
	text->setOutlineColor(sf::Color::White);
	text->setOutlineThickness(1.f);
	text->move(sf::Vector2f(0.f, (float)(text->getCharacterSize() * mTexts.size())));
	mTexts.push_back(text);
}

void TextRenderer::clearTextElements()
{
	for (auto it : mTexts)
		delete it;
	mTexts.clear();
}
