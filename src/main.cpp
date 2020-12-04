#include <cstdio>
#include <cstdlib>
#include <MyGame/MainView.hpp>
#include <MyGame/Config.hpp>

void terminating()
{
	axl::glw::cleanup();
	axl::game::View::cleanup();
	axl::game::Application::restoreDisplaySettings();
}

int main()
{
	using namespace MyGame;
	if(axl::glw::IERR_NONE != axl::glw::init(true))
	{
		fprintf(stderr, "> Initialization error!\n");
		return 1;
	}
	printf("OpenGL version %d.%d\n\n", axl::glw::MAJOR_GL_VERSION, axl::glw::MINOR_GL_VERSION);
	
	std::atexit(terminating);
	// Get the current display settings to see if it is set.
	axl::game::Application::enumDisplaySettings(&config.display_settings, axl::game::DisplaySetting::IP_CURRENT);
	printf("> DisplaySize(%dx%d)\n", config.display_settings.width, config.display_settings.height);

	MainView::DefaultCursor = axl::game::View::CUR_HAND;
	axl::math::Vec2i desktop_size(axl::game::Application::getCurrentDesktopSize());
	axl::math::Vec2i main_view_size(640, 480);
	axl::math::Vec2i main_view_position((desktop_size - main_view_size) / 2);
	
	MainView main_view(L"MyGame", main_view_position, main_view_size);
	// Create the view
	if(!main_view.create(false, MainView::VF_RESIZABLE, &config)) return 1;
	if(!main_view.isValid()) return 1;
	
	// Show view or die
	if(!main_view.show(MainView::SM_SHOW))
	{
		main_view.destroy();
		return 2;
	}

	// Event loop
	while(!axl::game::Application::IsQuitting)
	{
		if (axl::game::Application::pollEvent()) continue;
		if(!main_view.is_paused && main_view.main_context.makeCurrent())
		{
			main_view.render();
			main_view.swap();
		}
	}
	return 0;
}