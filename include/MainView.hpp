#pragma once
#include <ctime>
#include <axl.math/mat/Mat3f.hpp>
#include <axl.math/vec/Vec2f.hpp>
#include <axl.math/vec/Vec4f.hpp>
#include <axl.game/Application.hpp>
#include <axl.game/View.hpp>
#include <axl.game/Context.hpp>
#include <axl.glw/glw.hpp>
#include <axl.glw/gl.hpp>

namespace MyGame
{
	class MainView : public axl::game::View
	{
		public:
			MainView(const axl::util::WString& title, const axl::math::Vec2i& position, const axl::math::Vec2i& size, const Cursor& cursor = DefaultCursor);
			bool isValid() const;
			bool create(bool recreate = false, axl::game::View::Flags flags = axl::game::View::VF_FIXED, const MyGame::Config* game_config = 0);
			void destroy();
			bool init();
			void update();
			bool render(bool final = false);
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
			float m_scale;
			axl::game::Context m_main_context;
			axl::math::Vec2f m_origin;
			axl::math::Vec2f m_viewport_pos;
			axl::math::Vec2f m_viewport_size;
			axl::math::Vec2f m_projection_min;
			axl::math::Vec2f m_projection_max;
			axl::math::Mat3f m_viewport_to_screen;
			axl::math::Mat3f m_screen_to_viewport;
			struct UserData {
				bool is_animating;
				Cursor last_cursor;
				std::clock_t update_clock;
				double delta_time;
			} m_user_data;
			struct KeyData {
				bool f2_lock;
				bool f3_lock;
				bool f4_lock;
				bool space_lock;
			} m_key_data;
	};
} // namespace MyGame
