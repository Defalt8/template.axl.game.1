#pragma once
#include <ctime>
#include <axl.math/vec.hpp>
#include <axl.math/mat.hpp>
#include <axl.game/Application.hpp>
#include <axl.game/View.hpp>
#include <axl.game/Context.hpp>
#include <axl.glw/glw.hpp>
#include <axl.glw/gl.hpp>
#include "Projection.hpp"
#include "Viewport.hpp"

namespace MyGame
{

	class MainView : public axl::game::View
	{
		public:
			MainView(const axl::util::WString& title, const axl::math::Vec2i& position, const axl::math::Vec2i& size, const Cursor& cursor = DefaultCursor);
			bool isValid() const;
			bool create(bool recreate = false, axl::game::View::Flags flags = axl::game::View::VF_FIXED);
			void destroy();
			bool init();
			void update();
			bool render();
			axl::math::Vec2f screenToViewport(const axl::math::Vec2f& vec) const;
			axl::math::Vec2f viewportToScreen(const axl::math::Vec2f& vec) const;
		public: // Event callback methods
			bool onCreate(bool recreating);
			void onDestroy(bool recreating);
			void onMove(int x, int y);
			void onSize(int w, int h);
			void onPause();
			void onResume();
			void onKey(axl::game::KeyCode key_code, bool is_down);
			void onChar(char char_code);
			void onPointer(int index, int x, int y, bool is_down);
			void onPointerMove(int index, int x, int y);
		public:
			const axl::game::Context& main_context;
		private:
			axl::game::Context m_main_context;
			Viewport m_viewport;
			Projection m_projection;
			axl::math::Mat4f view_transform;
			axl::math::Mat4f triangle_transform;
			struct UserData {
				bool is_animating;
				bool are_axes_shown;
				bool is_cursor_shown;
				Cursor last_cursor;
				axl::math::Vec2f last_pointer_position;
				std::clock_t update_clock;
				double delta_time;
				struct Animated {
					double triangle_scale_angle;
					double triangle_rotation;
				} animated;
				axl::math::Vec2f origin, last_origin;
				double scale, last_scale;
			} m_user_data;
			struct KeyData {
				bool f1_lock;
				bool f2_lock;
				bool f3_lock;
				bool f4_lock;
				bool space_lock;
			} m_key_data;
	};
} // namespace MyGame
