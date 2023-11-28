#include "States.h"

int States::selectedWindow = 0;
int States::selectedPage = 0;
bool States::minimizedSidebar = true;
Mainframe* States::mainframe = nullptr;
SidebarMenu* States::sidebarMenu = nullptr;
Calendar* States::calendar = nullptr;
Internships* States::internships = nullptr;
Revision* States::revision = nullptr;
std::string States::jobName = "Senior Software Engineer";


wxBitmap States::LoadPNGFromResource(int resourceID, const wxString& resourceType)
{
	wxBitmap bitmap;
	HRSRC hResource = FindResource(nullptr, MAKEINTRESOURCE(resourceID), resourceType.wc_str());
	if (hResource != nullptr)
	{
		HGLOBAL hGlobal = LoadResource(nullptr, hResource);
		if (hGlobal != nullptr)
		{
			unsigned char* data = static_cast<unsigned char*>(LockResource(hGlobal));
			size_t size = SizeofResource(nullptr, hResource);
			wxMemoryInputStream memIStream(data, size);
			wxImage image(memIStream, wxBITMAP_TYPE_PNG);
			if (image.IsOk())
			{
				bitmap = wxBitmap(image);
			}
			UnlockResource(hGlobal);
			FreeResource(hGlobal);
		}
	};

	return bitmap;
};
