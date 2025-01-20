#pragma once


class GameRenderer : public Graphite::RendererInterface
{
public:
	GameRenderer() = default;
	virtual ~GameRenderer() = default;


	// RendererInterface implementation
	void Render() override;

};