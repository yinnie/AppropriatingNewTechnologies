#ifdef PARA_GUI_USED

#include "para.hpp"


//PARAGUI_CALLBACK(toggleHandler) {
bool toggleHandler(int id, PG_Widget* widget, unsigned long data, void *clientdata)
{

	// we can pass in some pointer to any userdata
	// (in this case we get a pointer to the application object)
	bool* toggle = (bool*) clientdata;
	
	*toggle = !*toggle;
	// return true to signal that we have processed this message
	return true;
}


para::para(bool *toggle)
{
	
		// every ParaGUI application need an application-object
	app = new PG_Application;

	// every application needs a theme (the look & feel of the widgets)
	app->LoadTheme("default");

	// we must initialize the screen where we want to draw ony

	app->InitScreen(200, 500, 32, SDL_SWSURFACE);

	// after that we can create some widgets
 
	rect = new PG_Rect(260, 100, 120, 50);
 
	myButton = new PG_Button (
								NULL,			// an optional parent widget for our button - NULL for no parent
								1,				// the widget id (used to identify events)
								*rect,			// the screen position where the button should appear
								"Play/Pause!"		
								);

	// this defines our callback handler for the message MSG_BUTTONCLICK,
	// we pass a pointer to the app object as userdata

	myButton->SetEventCallback(MSG_BUTTONCLICK, toggleHandler, toggle);

	myButton->Show();

	app->Run();

}

para::~para()
{
	app->Quit();
}
	
#endif // PARA_GUI_USED
