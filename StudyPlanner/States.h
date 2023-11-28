#pragma once
#include "Mainframe.h"
#include "SidebarMenu.h"
#include "Calendar.h"
#include "resource.h"
#include <wx/mstream.h>
#include "Revision.h"
#include "Internships.h"
class States
{
	public:
	static wxBitmap LoadPNGFromResource(int resourceID, const wxString& resourceType);
	static int selectedWindow;
	static bool minimizedSidebar;
	static Mainframe* mainframe;
	static SidebarMenu* sidebarMenu;
	static Calendar* calendar;
	static Internships* internships;
	static int selectedPage;
	static std::string companyretrieveurl;
	static std::string jobName;
	static Revision* revision;
};