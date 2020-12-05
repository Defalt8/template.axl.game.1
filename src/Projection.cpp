#include <cmath>
#include <axl.math/angle.hpp>
#include <axl.math/mat/Mat4d.hpp>
#include <Projection.hpp>

Projection::Projection(Type _type, double _left, double _right, double _bottom, double _top, double _near, double _far) :
	matrix(m_matrix),
	type(m_type),
	left(m_left),
	right(m_right),
	bottom(m_bottom),
	top(m_top),
	near(m_near),
	far(m_far)
{
	this->set(_type, _left, _right, _bottom, _top, _near, _far);
}
Projection::Projection(const Projection& projection) :
	matrix(m_matrix),
	type(m_type),
	left(m_left),
	right(m_right),
	bottom(m_bottom),
	top(m_top),
	near(m_near),
	far(m_far),
	m_matrix(projection.m_matrix),
	m_type(projection.m_type),
	m_left(projection.m_left),
	m_right(projection.m_right),
	m_bottom(projection.m_bottom),
	m_top(projection.m_top),
	m_near(projection.m_near),
	m_far(projection.m_far)
{}
Projection& Projection::operator=(const Projection& projection)
{
	m_matrix = projection.m_matrix;
	m_type = projection.m_type;
	m_left = projection.m_left;
	m_right = projection.m_right;
	m_bottom = projection.m_bottom;
	m_top = projection.m_top;
	m_near = projection.m_near;
	m_far = projection.m_far;
	return *this;
}
void Projection::set(Type _type, double _left, double _right, double _bottom, double _top, double _near, double _far)
{
	m_type = _type;
	m_left = _left;
	m_right = _right;
	m_bottom = _bottom;
	m_top = _top;
	m_near = _near;
	m_far = _far;
	this->calculate();
}
void Projection::setPerspective(double fov, double ratio, double _near, double _far)
{
	using namespace axl::math;
	m_type = PT_PERSPECTIVE;
	const double ny = (near * std::tan(Angle::degToRad(fov))) / 2.0;
	const double nx = ratio * ny;
	m_bottom = -ny;
	m_top = ny;
	m_left = -nx;
	m_right = nx;
	m_near = _near;
	m_far = _far;
	m_ratio = ratio;
	this->calculate();
}
void Projection::setPerspective(double fov)
{
	using namespace axl::math;
	m_type = PT_PERSPECTIVE;
	const double ny = (near * std::tan(Angle::degToRad(fov))) / 2.0;
	const double nx = m_ratio * ny;
	m_bottom = -ny;
	m_top = ny;
	m_left = -nx;
	m_right = nx;
	this->calculate();
}
void Projection::setType(Type _type)
{
	if(_type == m_type) return;
	m_type = _type;
	this->calculate();
}
void Projection::setLeftRight(double _left, double _right)
{
	m_left = _left;
	m_right = _right;
	this->calculate();
}
void Projection::setBottomTop(double _bottom, double _top)
{
	m_bottom = _bottom;
	m_top = _top;
	this->calculate();
}
void Projection::setNearFar(double _near, double _far)
{
	m_near = _near;
	m_far = _far;
	this->calculate();
}
const axl::math::Mat4d& Projection::calculate()
{
	switch(m_type)
	{
		default:
		case PT_ORTHOGRAPHIC:
			return this->m_matrix.set(
				(2.0 / (right - left)), 0, 0, 0,
				0, (2.0 / (top - bottom)), 0, 0,
				0, 0, (-2.0 / (far - near)), 0,
				(-(right + left) / (right - left)), (-(top + bottom) / (top - bottom)), (-(far + near) / (far - near)), 1
			);
		case PT_PERSPECTIVE:
			return this->m_matrix.set(
				(2.0 * near / (right - left)), 0, 0, 0,
				0, (2.0 * near / (top - bottom)), 0, 0,
				((right + left) / (right - left)), ((top + bottom) / (top - bottom)), (-(far + near) / (far - near)), -1,
				0, 0, (-(2 * far * near) / (far - near)), 0
			);
	}
}

Projection::Type Projection::DefaultType = Projection::PT_ORTHO;
Projection Projection::Default(DefaultType, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0);