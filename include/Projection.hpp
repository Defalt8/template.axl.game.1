#pragma once
#include <axl.math/mat/Mat4d.hpp>

class Projection
{
	public:
		enum Type { PT_ORTHOGRAPHIC = 0, PT_ORTHO = 0, PT_PERSPECTIVE = 1, PT_PERSP = 1 };
		static Type DefaultType;
	public:
		Projection(Type type, double left, double right, double bottom, double top, double near, double far);
		Projection(const Projection& projection = Default);
		Projection& operator=(const Projection& projection);
		void set(Type type, double left, double right, double bottom, double top, double near, double far);
		void setPerspective(double fov, double ratio, double near, double far);
		void setPerspective(double fov);
		void setType(Type type);
		void setLeftRight(double left, double right);
		void setBottomTop(double bottom, double top);
		void setNearFar(double near, double far);
		const axl::math::Mat4d& calculate();
	public:
		const Type& type;
		const double &left, &right, &bottom, &top, &near, &far;
		const axl::math::Mat4d& matrix;
		static Projection Default;
	private:
		Type m_type;
		double m_left, m_right, m_bottom, m_top, m_near, m_far, m_ratio;
		axl::math::Mat4d m_matrix;
};