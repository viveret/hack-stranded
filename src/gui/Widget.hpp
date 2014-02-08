#ifndef SURVIVOR_ENGINE_GUI_WIDGET
#define SURVIVOR_ENGINE_GUI_WIDGET

// Basic building block for GUI elements

#include "../Types.hpp"
#include "../math/Vector.hpp"
#include <string>

namespace SE_GUI
{
	struct Widget
	{
		Widget();
		virtual void Cleanup() {}

		Vector2i Position, Size; // Always top left
		uchar Gravity;
		bool Enabled;

		uint m_Texture;

		std::string Label_Text;
		uchar Label_Gravity;

		virtual uchar Type() = 0;

		virtual void Update() {}
		virtual void Render() {}
	};

	namespace Widget_Types
	{enum{

		TYPE_UNKNOWN = 0,

		TYPE_BUTTON,
		TYPE_SLIDER,
		TYPE_LIST_BOX,
		TYPE_LIST_DOWN,
		TYPE_COMBO_BOX,
		TYPE_ICON,
		TYPE_VIEW_TREE,
		TYPE_VIEW_GRID,

		// Navigation of GUI / containers
		TYPE_TAB,
		TYPE_SCROLLBAR,
		TYPE_FRAME,
		TYPE_BACKGROUND, // Frame + Texture

		TYPE_INPUT,

		TYPE_BAR_STATUS,
		TYPE_BAR_PROGRESS,
		TYPE_BAR_INFO,

		TYPE_SPINNER,
	};}
}






#endif
