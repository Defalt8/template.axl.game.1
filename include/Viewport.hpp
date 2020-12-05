#pragma once
#include <axl.math/vec/Vec2i.hpp>

namespace MyGame { class MainView; }

class Viewport
{
	public:
		Viewport(int x, int y, int width, int height);
		Viewport(const axl::math::Vec2i& position = axl::math::Vec2i::Zero, const axl::math::Vec2i& size = axl::math::Vec2i::Identity);
		Viewport(const Viewport& viewport);
	private:
		void set(int x, int y, int width, int height);
		void set(const axl::math::Vec2i& position, const axl::math::Vec2i& size);
	public:
		const axl::math::Vec2i& position;
		const axl::math::Vec2i& size;
	private:
		axl::math::Vec2i m_position;
		axl::math::Vec2i m_size;
		friend class MyGame::MainView;
};