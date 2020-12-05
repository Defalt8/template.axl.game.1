#include <axl.math/vec/Vec2i.hpp>
#include <Viewport.hpp>

Viewport::Viewport(int x, int y, int width, int height) :
	position(m_position),
	size(m_size),
	m_position(x, y),
	m_size(width, height)
{}
Viewport::Viewport(const axl::math::Vec2i& _position, const axl::math::Vec2i& _size) :
	position(m_position),
	size(m_size),
	m_position(_position),
	m_size(_size)
{}
Viewport::Viewport(const Viewport& viewport) :
	position(m_position),
	size(m_size),
	m_position(viewport.m_position),
	m_size(viewport.m_size)
{}
void Viewport::set(int x, int y, int width, int height)
{
	m_position.set(x, y);
	m_size.set(width, height);
}
void Viewport::set(const axl::math::Vec2i& _position, const axl::math::Vec2i& _size)
{
	m_position = _position;
	m_size = _size;
}
