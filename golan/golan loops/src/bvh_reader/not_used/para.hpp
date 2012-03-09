
#ifndef PARA_HPP
#define PARA_HPP

#include "pgapplication.h"
#include "pgbutton.h"

/// Contains all paraGUI specific code
/** Later I'd like to write a perl script that
	will find flagged members in the hpp files
	and write the generic gui code for manipulating
	and viewing their values, and a hierarchical method
	of browsing all the manipulable members.
*/
class para 
{
public:
	para(bool *toggle);
	~para();
	
private:
	//bool toggleHandler(int id, PG_Widget* widget, unsigned long data, void *clientdata);
	PG_Application *app;
	
	PG_Rect *rect;
	PG_Button *myButton;
};

#endif // PARA_HPP
