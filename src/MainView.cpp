#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <MainView.hpp>
#include <axl.glw/gl1.hpp>
#include <axl.math/constants.hpp>
#include <axl.math/angle.hpp>
#include <axl.math/util.hpp>
#include <glerr.hpp>

namespace MyGame {

const static double CLOCK_FACTOR = (double)1.0 / (double)CLOCKS_PER_SEC;

MainView::MainView(const axl::util::WString& _title, const axl::math::Vec2i& _position, const axl::math::Vec2i& _size, const axl::game::View::Cursor& _cursor) :
	axl::game::View(_title, _position, _size, _cursor),
	main_context(m_main_context),
	m_main_context()
{}

bool MainView::isValid() const
{
	return axl::game::View::isValid();
}

bool MainView::create(bool recreate, axl::game::View::Flags flags)
{
	const axl::game::View::Config view_configs[] = {
		axl::game::View::Config(1, axl::game::View::Config::PT_RGBA, 32,8,8,8,8, 24,8, 0,0,0,0,0, 64, true, false),
		axl::game::View::Config(2, axl::game::View::Config::PT_RGBA, 32,8,8,8,8, 24,8, 0,0,0,0,0, 32, true, false),
		axl::game::View::Config(3, axl::game::View::Config::PT_RGBA, 32,8,8,8,8, 24,8, 0,0,0,0,0, 16, true, false),
		axl::game::View::Config(4, axl::game::View::Config::PT_RGBA, 32,8,8,8,8, 24,8, 0,0,0,0,0, 8, true, false),
		axl::game::View::Config(5, axl::game::View::Config::PT_RGBA, 32,8,8,8,8, 24,8, 0,0,0,0,0, 4, true, false),
		axl::game::View::Config(6, axl::game::View::Config::PT_RGBA, 32,8,8,8,8, 24,8, 0,0,0,0,0, 0, true, false),
		axl::game::View::Config(7, axl::game::View::Config::PT_RGB, 24,8,8,8,0, 24,8, 0,0,0,0,0, 0, true, false),
		axl::game::View::Config(8, axl::game::View::Config::PT_RGB, 24,8,8,8,0, 24,8, 0,0,0,0,0, 0, false, false)
	};
	int view_configs_count = sizeof(view_configs) / sizeof(axl::game::View::Config);

	const axl::game::Context::Config main_context_configs[] = {
		axl::game::Context::Config(1, 4, 0, axl::game::Context::Config::GLP_COMPATIBLITY),
		axl::game::Context::Config(2, 3, 0, axl::game::Context::Config::GLP_COMPATIBLITY),
		axl::game::Context::Config(3, 2, 0, axl::game::Context::Config::GLP_COMPATIBLITY),
		axl::game::Context::Config(4, 1, 0, axl::game::Context::Config::GLP_COMPATIBLITY),
		axl::game::Context::Config(5, 0, 0, axl::game::Context::Config::GLP_COMPATIBLITY)
	};
	int main_context_configs_count = sizeof(main_context_configs) / sizeof(axl::game::Context::Config);

	if(!axl::game::View::create(recreate, view_configs, view_configs_count, flags))
		return false;
	if(!m_main_context.create(recreate, this, main_context_configs, main_context_configs_count))
		return false;
	return this->init();
}

void MainView::destroy()
{
	axl::game::View::destroy();
}

bool MainView::init()
{
	m_scale = 1.0;
	m_origin.set(0.0f, 0.0f);
	m_viewport_pos.set(0.0f, 0.0f);
	m_viewport_to_screen = axl::math::Mat3f::Identity;
	m_screen_to_viewport = axl::math::Mat3f::Identity;
	m_projection_min.set(-1.0f, -1.0f);
	m_projection_max.set(1.0f, 1.0f);
	// user data
	m_user_data.is_animating = true;
	m_user_data.are_axes_shown = true;
	m_user_data.is_cursor_shown = false;
	m_user_data.last_cursor = DefaultCursor;
	m_user_data.update_clock = std::clock();
	m_user_data.delta_time = 0.0;
	m_user_data.is_cursor_shown = true;
	this->setCursor(m_user_data.is_cursor_shown ? m_user_data.last_cursor : CUR_NONE);
	// key data
	m_key_data.f2_lock = false;
	m_key_data.f3_lock = false;
	m_key_data.f4_lock = false;
	m_key_data.space_lock = false;
	if(this->isValid() && this->m_main_context.isValid())
	{
		if(m_main_context.makeCurrent())
		{
			// opengl initialization
			using namespace axl::glw::gl;
			using namespace axl::glw::gl1;
			GLE(glShadeModel(GL_FLAT));
			GLE(glEnable(GL_BLEND));
			GLE(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
			return true;
		}
	}
	return false;
}

void MainView::update()
{
	if(this->is_paused || !this->m_user_data.is_animating) return;
	// calculate delta time
	m_user_data.delta_time = (double)((double)std::clock() - m_user_data.update_clock) * CLOCK_FACTOR;
	m_user_data.update_clock = std::clock();
	// update
	static double scale_freq = 0.0;
	scale_freq += axl::math::Angle::degToRad(90.0) * m_user_data.delta_time;
	m_scale = axl::math::Util::map(std::sin(scale_freq), -1.0, 1.0, 0.0, 1.0);
}

bool MainView::render()
{
	if(m_main_context.makeCurrent())
	{
		using namespace axl::glw::gl;
		glClearColor(0.01f, 0.01f, 0.02f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(m_origin.x, m_origin.y, 0.0);
		glScalef(m_scale, m_scale, m_scale);
		if(axl::glw::gl1::V_1_1)
		{
			// Axes
			if(m_user_data.are_axes_shown)
			{
				glPushMatrix();
				glBegin(GL_LINES);
					glColor4d(1.0,0.1,0.1,0.69);
					glVertex2f(-1000.0f, 0.0f);
					glVertex2f( 1000.0f, 0.0f);
					glColor4d(0.1,1.0,0.1,0.69);
					glVertex2f(0.0f, -1000.0f);
					glVertex2f(0.0f,  1000.0f);
				glEnd();
				glPopMatrix();
			}
			// Triangle
			glPushMatrix();
			glColor4d(1.0,1.0,1.0,1.0);
			glBegin(GL_TRIANGLES);
				glVertex2f(-0.5f, -0.5f);
				glVertex2f( 0.5f, -0.5f);
				glVertex2f( 0.0f,  0.5f);
			glEnd();
			glPopMatrix();
		}
		return true;
	}
	return false;
}

axl::math::Vec2f MainView::screenToViewport(const axl::math::Vec2f& vec) const
{
	return axl::math::Vec2f(
		m_projection_min.x + vec.x * (m_projection_max.x - m_projection_min.x) / m_viewport_size.x,
		m_projection_min.y + (size.y - vec.y) * (m_projection_max.y - m_projection_min.y) / m_viewport_size.y
	);
}

axl::math::Vec2f MainView::viewportToScreen(const axl::math::Vec2f& vec) const
{
	return axl::math::Vec2f(
		(vec.x - m_projection_min.x) * m_viewport_size.x / (m_projection_max.x - m_projection_min.x),
		size.y - ((vec.y - m_projection_min.y) * m_viewport_size.y / (m_projection_max.y - m_projection_min.y))
	);
}


bool MainView::onCreate(bool recreating)
{
	return axl::game::View::onCreate(recreating);
}

void MainView::onDestroy(bool recreating)
{
	m_main_context.destroy();
	axl::game::View::onDestroy();
	if(!recreating)
		axl::game::Application::quit(0);
}

void MainView::onMove(int x, int y)
{
	axl::game::View::onMove(x, y);
}

void MainView::onSize(int w, int h)
{
	axl::game::View::onSize(w, h);
	if(this->isValid() && this->m_main_context.isValid())
	{
		if(m_main_context.makeCurrent())
		{
			using namespace axl::glw::gl;
			float viewport_x = 0.0f, viewport_y = 0.0f, viewport_w = (float)w, viewport_h = (float)h;
			m_viewport_pos.set(viewport_x, viewport_y);
			m_viewport_size.set(viewport_w, viewport_h);
			glViewport((int)viewport_x, (int)viewport_y, (int)viewport_w, (int)viewport_h);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			if(w >= h)
			{
				const float ratio = (float)w/h;
				m_projection_min.set(-ratio, -1.0);
				m_projection_max.set(ratio, 1.0);
			}
			else
			{
				const float ratio = (float)h/w;
				m_projection_min.set(-1.0, -ratio);
				m_projection_max.set(1.0, ratio);
			}
			glOrtho((double)m_projection_min.x, (double)m_projection_max.x, (double)m_projection_min.y, (double)m_projection_max.y, -1.0, 1.0);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}
	}
}

void MainView::onPause()
{
	axl::game::View::onPause();
	m_user_data.is_animating = false;
}

void MainView::onResume()
{
	axl::game::View::onResume();
	m_user_data.is_animating = true;
	m_user_data.update_clock = std::clock();
}

void MainView::onKey(axl::game::KeyCode key_code, bool is_down)
{
	if(is_down)
	{
		switch(key_code)
		{
			case axl::game::KEY_ESCAPE:
				this->destroy();
				break;
			case axl::game::KEY_F1:
				if(!m_key_data.f1_lock)
				{
					m_user_data.are_axes_shown = !m_user_data.are_axes_shown;
					m_key_data.f1_lock = true;
				}
				break;
			case axl::game::KEY_F2:
				if(!m_key_data.f2_lock)
				{
					this->show((this->visiblity == axl::game::View::VS_FULLSCREEN ? axl::game::View::SM_SHOW : axl::game::View::SM_FULLSCREEN));
					m_key_data.f2_lock = true;
				}
				break;
			case axl::game::KEY_F3:
				if(!m_key_data.f3_lock)
				{
					m_user_data.is_cursor_shown = !m_user_data.is_cursor_shown;
					this->setCursor(m_user_data.is_cursor_shown ? m_user_data.last_cursor : CUR_NONE);
					m_key_data.f3_lock = true;
				}
				break;
			case axl::game::KEY_F4:
				if(!m_key_data.f4_lock)
				{
					m_key_data.f4_lock = true;
				}
				break;
			case axl::game::KEY_SPACE:
				if(!m_key_data.space_lock)
				{
					// last render before pause
					if(this->m_user_data.is_animating)
					{
						this->update();
						this->render();
						this->swap();
					}
					this->m_user_data.is_animating = !this->m_user_data.is_animating;
					m_user_data.update_clock = std::clock();
					m_key_data.space_lock = true;
				}
				break;
		}
	}
	else
	{
		switch(key_code)
		{
			case axl::game::KEY_F1: m_key_data.f1_lock = false; break;
			case axl::game::KEY_F2: m_key_data.f2_lock = false; break;
			case axl::game::KEY_F3: m_key_data.f3_lock = false; break;	
			case axl::game::KEY_F4: m_key_data.f4_lock = false; break;	
			case axl::game::KEY_SPACE: m_key_data.space_lock = false; break;
		}
	}
	axl::game::View::onKey(key_code, is_down);
}

void MainView::onChar(char char_code)
{
	axl::game::View::onChar(char_code);
}

void MainView::onPointer(int index, int x, int y, bool is_down)
{
	axl::game::View::onPointer(index, x, y, is_down);
	if(is_down)
	{
		// capture cursor when down
		this->capturePointer(true);
		// cursor hides when down
		if(m_user_data.is_cursor_shown)
			this->setCursor(axl::game::View::CUR_CROSS);
		const axl::math::Vec2f screen_position((float)x, (float)y);
		const axl::math::Vec2f viewport_position = this->screenToViewport(screen_position);
		// left click - down
		if(index == axl::game::View::PI_LEFT_BUTTON)
		{
			this->m_origin = viewport_position;
		}
	}
	else
	{
		// cursor shows when released
		if(m_user_data.is_cursor_shown)
			this->setCursor(m_user_data.last_cursor);
		// release cursor when released
		this->capturePointer(false);
	}
}

void MainView::onPointerMove(int index, int x, int y)
{
	axl::game::View::onPointerMove(index, x, y);
	const axl::math::Vec2f screen_position((float)x, (float)y);
	const axl::math::Vec2f viewport_position = this->screenToViewport(screen_position);
	// left click or touch-1
	if(pointers[axl::game::View::PI_LEFT_BUTTON])
	{
		this->m_origin = viewport_position;
	}
}

} // namespace MyGame
