///////////////////////////////////////////////////////////////////////////////
//
// wxFormBuilder - A Visual Dialog Editor for wxWidgets.
// Copyright (C) 2005 José Antonio Hurtado
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// Written by
//   José Antonio Hurtado - joseantonio.hurtado@gmail.com
//   Juan Antonio Ortega  - jortegalalmolda@gmail.com
//
///////////////////////////////////////////////////////////////////////////////

#include <component.h>
#include <plugin.h>
#include <xrcconv.h>
#include <ticpp.h>

#include <wx/calctrl.h>
#include <wx/html/htmlwin.h>
#include <wx/treectrl.h>
#include <wx/spinbutt.h>
#include <wx/spinctrl.h>
#include <wx/tglbtn.h>
#include <wx/scrolbar.h>
#include <wx/checklst.h>
#include <wx/datectrl.h>
#include <wx/grid.h>
#include <wx/dirctrl.h>

#if wxCHECK_VERSION( 2, 8, 0 )
	#include <wx/richtext/richtextctrl.h>
	#include "logo.xpm"
	#include "smiley.xpm"
	#include <wx/clrpicker.h>
	#include <wx/fontpicker.h>
	#include <wx/filepicker.h>
	#include <wx/hyperlink.h>
#endif

///////////////////////////////////////////////////////////////////////////////

/**
Event handler for events generated by controls in this plugin
*/
class ComponentEvtHandler : public wxEvtHandler
{
private:
	wxWindow* m_window;
	IManager* m_manager;

public:
	ComponentEvtHandler( wxWindow* win, IManager* manager )
	:
	m_window( win ),
	m_manager( manager )
	{
	}

protected:
	void OnGridClick( wxGridEvent& event );
	void OnGridColSize( wxGridSizeEvent& event );
	void OnGridRowSize( wxGridSizeEvent& event );
	#if wxCHECK_VERSION( 2, 8, 0 )
		void OnColourPickerColourChanged( wxColourPickerEvent& event );
		void OnFontPickerFontChanged( wxFontPickerEvent& event );
		void OnFilePickerFileChanged( wxFileDirPickerEvent& event );
		void OnDirPickerDirChanged( wxFileDirPickerEvent& event );
	#endif
	void OnGenericDirCtrlExpandItem( wxTreeEvent& event );
	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE( ComponentEvtHandler, wxEvtHandler )
	#if wxCHECK_VERSION( 2, 8, 0 )
		EVT_COLOURPICKER_CHANGED( -1, ComponentEvtHandler::OnColourPickerColourChanged )
		EVT_FONTPICKER_CHANGED( -1, ComponentEvtHandler::OnFontPickerFontChanged )
		EVT_FILEPICKER_CHANGED( -1, ComponentEvtHandler::OnFilePickerFileChanged )
		EVT_DIRPICKER_CHANGED( -1, ComponentEvtHandler::OnDirPickerDirChanged )
	#endif
	// Grid also seems to ignore clicks
	EVT_GRID_CELL_LEFT_CLICK( ComponentEvtHandler::OnGridClick )
	EVT_GRID_LABEL_LEFT_CLICK( ComponentEvtHandler::OnGridClick )

	EVT_GRID_COL_SIZE( ComponentEvtHandler::OnGridColSize )
	EVT_GRID_ROW_SIZE( ComponentEvtHandler::OnGridRowSize )
END_EVENT_TABLE()

/**
Event handler for events generated by wxGenericDirCtrl.
*/
class GenericDirCtrlEvtHandler : public wxEvtHandler
{
public:
	GenericDirCtrlEvtHandler( wxWindow* win, IManager* manager )
	:
	m_window( win ),
	m_manager( manager )
	{
	}

protected:
	void OnGenericDirCtrlLeftClick( wxMouseEvent& event );
	DECLARE_EVENT_TABLE()
private:
	wxWindow* m_window;
	IManager* m_manager;

};

BEGIN_EVENT_TABLE( GenericDirCtrlEvtHandler, wxEvtHandler )
	// GenericDirCtrl also seems to ignore clicks
	EVT_LEFT_DOWN( GenericDirCtrlEvtHandler::OnGenericDirCtrlLeftClick )
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////

class CalendarCtrlComponent : public ComponentBase
{
public:
	wxObject* Create(IObject *obj, wxObject *parent)
	{
		return new wxCalendarCtrl((wxWindow *)parent,-1,
			wxDefaultDateTime,
			obj->GetPropertyAsPoint(_("pos")),
			obj->GetPropertyAsSize(_("size")),
			obj->GetPropertyAsInteger(_("style")) | obj->GetPropertyAsInteger(_("window_style")));
	}

	ticpp::Element* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxCalendarCtrl"), obj->GetPropertyAsString(_("name")));
		xrc.AddWindowProperties();
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc( ticpp::Element* xrcObj )
	{
		XrcToXfbFilter filter(xrcObj, _("wxCalendarCtrl"));
		filter.AddWindowProperties();
		return filter.GetXfbObject();
	}
};

class DatePickerCtrlComponent : public ComponentBase
{
public:
	wxObject* Create(IObject *obj, wxObject *parent)
	{
		return new wxDatePickerCtrl((wxWindow *)parent,-1,
			wxDefaultDateTime,
			obj->GetPropertyAsPoint(_("pos")),
			obj->GetPropertyAsSize(_("size")),
			obj->GetPropertyAsInteger(_("style")) | obj->GetPropertyAsInteger(_("window_style")));
	}

	ticpp::Element* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxDatePickerCtrl"), obj->GetPropertyAsString(_("name")));
		xrc.AddWindowProperties();
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc( ticpp::Element* xrcObj )
	{
		XrcToXfbFilter filter(xrcObj, _("wxDatePickerCtrl"));
		filter.AddWindowProperties();
		return filter.GetXfbObject();
	}
};

class RichTextCtrlComponent : public ComponentBase
{
public:
	wxObject* Create( IObject* obj, wxObject* parent )
	{
		wxRichTextCtrl* richText = new wxRichTextCtrl( 	(wxWindow*)parent,
															wxID_ANY,
															wxEmptyString,
															obj->GetPropertyAsPoint(_("pos")),
															obj->GetPropertyAsSize(_("size")),
															obj->GetPropertyAsInteger(_("style")) | obj->GetPropertyAsInteger(_("window_style")));

		wxFont textFont = wxFont(12, wxROMAN, wxNORMAL, wxNORMAL);
		wxFont boldFont = wxFont(12, wxROMAN, wxNORMAL, wxBOLD);
		wxFont italicFont = wxFont(12, wxROMAN, wxITALIC, wxNORMAL);

		wxFont font(12, wxROMAN, wxNORMAL, wxNORMAL);

		wxRichTextCtrl& r = *richText;
		r.SetFont(font);
		r.BeginSuppressUndo();

		r.BeginParagraphSpacing(0, 20);

		r.BeginAlignment(wxTEXT_ALIGNMENT_CENTRE);
		r.BeginBold();

		r.BeginFontSize(14);
		r.WriteText(wxT("Welcome to wxRichTextCtrl, a wxWidgets control for editing and presenting styled text and images"));
		r.EndFontSize();
		r.Newline();

		r.BeginItalic();
		r.WriteText(wxT("by Julian Smart"));
		r.EndItalic();

		r.EndBold();

		r.Newline();
		r.WriteImage(wxBitmap(logo_xpm));

		r.EndAlignment();

		r.Newline();

		r.WriteText(wxT("What can you do with this thing? "));
		r.WriteImage(wxBitmap(smiley_xpm));
		r.WriteText(wxT(" Well, you can change text "));

		r.BeginTextColour(wxColour(255, 0, 0));
		r.WriteText(wxT("colour, like this red bit."));
		r.EndTextColour();

		r.BeginTextColour(wxColour(0, 0, 255));
		r.WriteText(wxT(" And this blue bit."));
		r.EndTextColour();

		r.WriteText(wxT(" Naturally you can make things "));
		r.BeginBold();
		r.WriteText(wxT("bold "));
		r.EndBold();
		r.BeginItalic();
		r.WriteText(wxT("or italic "));
		r.EndItalic();
		r.BeginUnderline();
		r.WriteText(wxT("or underlined."));
		r.EndUnderline();

		r.BeginFontSize(14);
		r.WriteText(wxT(" Different font sizes on the same line is allowed, too."));
		r.EndFontSize();

		r.WriteText(wxT(" Next we'll show an indented paragraph."));

		r.BeginLeftIndent(60);
		r.Newline();

		r.WriteText(wxT("Indented paragraph."));
		r.EndLeftIndent();

		r.Newline();

		r.WriteText(wxT("Next, we'll show a first-line indent, achieved using BeginLeftIndent(100, -40)."));

		r.BeginLeftIndent(100, -40);
		r.Newline();

		r.WriteText(wxT("It was in January, the most down-trodden month of an Edinburgh winter."));
		r.EndLeftIndent();

		r.Newline();

		r.WriteText(wxT("Numbered bullets are possible, again using subindents:"));

		r.BeginNumberedBullet(1, 100, 60);
		r.Newline();

		r.WriteText(wxT("This is my first item. Note that wxRichTextCtrl doesn't automatically do numbering, but this will be added later."));
		r.EndNumberedBullet();

		r.BeginNumberedBullet(2, 100, 60);
		r.Newline();

		r.WriteText(wxT("This is my second item."));
		r.EndNumberedBullet();

		r.Newline();

		r.WriteText(wxT("The following paragraph is right-indented:"));

		r.BeginRightIndent(200);
		r.Newline();

		r.WriteText(wxT("It was in January, the most down-trodden month of an Edinburgh winter. An attractive woman came into the cafe, which is nothing remarkable."));
		r.EndRightIndent();

		r.Newline();

		wxArrayInt tabs;
		tabs.Add(400);
		tabs.Add(600);
		tabs.Add(800);
		tabs.Add(1000);
		wxTextAttrEx attr;
		attr.SetFlags(wxTEXT_ATTR_TABS);
		attr.SetTabs(tabs);
		r.SetDefaultStyle(attr);

		r.WriteText(wxT("This line contains tabs:\tFirst tab\tSecond tab\tThird tab"));

		r.Newline();
		r.WriteText(wxT("Other notable features of wxRichTextCtrl include:"));

		r.BeginSymbolBullet(wxT('*'), 100, 60);
		r.Newline();
		r.WriteText(wxT("Compatibility with wxTextCtrl API"));
		r.EndSymbolBullet();

		r.WriteText(wxT("\nNote: this content was generated programmatically and copied from the sample. The images were loaded from inline XPMs. Enjoy wxRichTextCtrl!"));

		r.EndSuppressUndo();

		return richText;
	}

	ticpp::Element* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("unknown"), obj->GetPropertyAsString(_("name")));
		//xrc.AddWindowProperties();
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc( ticpp::Element* xrcObj )
	{
		XrcToXfbFilter filter(xrcObj, _("wxRichTextCtrl"));
		filter.AddWindowProperties();
		return filter.GetXfbObject();
	}
};

class HtmlWindowComponent : public ComponentBase
{
public:
	wxObject* Create(IObject *obj, wxObject *parent)
	{
		wxHtmlWindow *hw = new wxHtmlWindow((wxWindow *)parent,-1,
			obj->GetPropertyAsPoint(_("pos")),
			obj->GetPropertyAsSize(_("size")),
			obj->GetPropertyAsInteger(_("style")) | obj->GetPropertyAsInteger(_("window_style")));

		wxString dummy_page(
			wxT("<b>wxHtmlWindow</b><br />")
			wxT("This is a dummy page.</body></html>"));

		hw->SetPage(dummy_page);

		return hw;
	}

	ticpp::Element* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxHtmlWindow"), obj->GetPropertyAsString(_("name")));
		xrc.AddWindowProperties();
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc( ticpp::Element* xrcObj )
	{
		XrcToXfbFilter filter(xrcObj, _("wxHtmlWindow"));
		filter.AddWindowProperties();
		return filter.GetXfbObject();
	}
};



class ToggleButtonComponent : public ComponentBase
{
public:
	wxObject* Create(IObject *obj, wxObject *parent)
	{
		wxToggleButton* tb = new wxToggleButton((wxWindow *)parent,-1,
			obj->GetPropertyAsString(_("label")),
			obj->GetPropertyAsPoint(_("pos")),
			obj->GetPropertyAsSize(_("size")),
			obj->GetPropertyAsInteger(_("window_style")));

		tb->SetValue( ( obj->GetPropertyAsInteger(_("value")) != 0 ) );
		return tb;
	}

	ticpp::Element* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxToggleButton"), obj->GetPropertyAsString(_("name")));
		xrc.AddWindowProperties();
		xrc.AddProperty(_("label"),_("label"), XRC_TYPE_TEXT);
		xrc.AddProperty(_("value"),_("checked"), XRC_TYPE_BOOL);
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc( ticpp::Element* xrcObj )
	{
		XrcToXfbFilter filter(xrcObj, _("wxToggleButton"));
		filter.AddWindowProperties();
		filter.AddProperty(_("label"),_("label"), XRC_TYPE_TEXT);
		filter.AddProperty(_("checked"),_("value"), XRC_TYPE_BOOL);
		return filter.GetXfbObject();
	}

};

class TreeCtrlComponent : public ComponentBase
{
public:
	wxObject* Create(IObject *obj, wxObject *parent)
	{
		int style = obj->GetPropertyAsInteger(_("style"));
		wxTreeCtrl *tc = new wxTreeCtrl((wxWindow *)parent,-1,
			obj->GetPropertyAsPoint(_("pos")),
			obj->GetPropertyAsSize(_("size")),
			style | obj->GetPropertyAsInteger(_("window_style")));

		// dummy nodes
		wxTreeItemId root = tc->AddRoot(wxT("root node"));
		wxTreeItemId node1 = tc->AppendItem(root,wxT("node1"));
		wxTreeItemId node2 = tc->AppendItem(root,wxT("node2"));
		wxTreeItemId node3 = tc->AppendItem(node2,wxT("node3"));
		if ( ( style & wxTR_HIDE_ROOT ) == 0 )
		{
			tc->Expand(root);
		}
		tc->Expand(node1);
		tc->Expand(node2);
		tc->Expand(node3);

		return tc;
	}

	ticpp::Element* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxTreeCtrl"), obj->GetPropertyAsString(_("name")));
		xrc.AddWindowProperties();
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc( ticpp::Element* xrcObj )
	{
		XrcToXfbFilter filter(xrcObj, _("wxTreeCtrl"));
		filter.AddWindowProperties();
		return filter.GetXfbObject();
	}
};

class ScrollBarComponent : public ComponentBase
{
public:
	wxObject* Create(IObject *obj, wxObject *parent)
	{
		wxScrollBar *sb = new wxScrollBar((wxWindow *)parent,-1,
			obj->GetPropertyAsPoint(_("pos")),
			obj->GetPropertyAsSize(_("size")),
			obj->GetPropertyAsInteger(_("style")) | obj->GetPropertyAsInteger(_("window_style")));

        sb->SetScrollbar(obj->GetPropertyAsInteger(_T("value")),
            obj->GetPropertyAsInteger(_T("thumbsize")),
            obj->GetPropertyAsInteger(_T("range")),
            obj->GetPropertyAsInteger(_T("pagesize")));
        return sb;
	}

	ticpp::Element* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxScrollBar"), obj->GetPropertyAsString(_("name")));
		xrc.AddWindowProperties();
		xrc.AddProperty(_("value"), _("value"), XRC_TYPE_INTEGER);
		xrc.AddProperty(_("thumbsize"), _("thumbsize"), XRC_TYPE_INTEGER);
		xrc.AddProperty(_("range"), _("range"), XRC_TYPE_INTEGER);
		xrc.AddProperty(_("pagesize"), _("pagesize"), XRC_TYPE_INTEGER);
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc( ticpp::Element* xrcObj )
	{
		XrcToXfbFilter filter(xrcObj, _("wxScrollBar"));
		filter.AddWindowProperties();
		filter.AddProperty(_("value"), _("value"), XRC_TYPE_INTEGER);
		filter.AddProperty(_("thumbsize"), _("thumbsize"), XRC_TYPE_INTEGER);
		filter.AddProperty(_("range"), _("range"), XRC_TYPE_INTEGER);
		filter.AddProperty(_("pagesize"), _("pagesize"), XRC_TYPE_INTEGER);
		return filter.GetXfbObject();
	}
};

class SpinCtrlComponent : public ComponentBase
{
public:
	wxObject* Create(IObject *obj, wxObject *parent)
	{
		return new wxSpinCtrl((wxWindow *)parent,-1,
			obj->GetPropertyAsString(_("value")),
			obj->GetPropertyAsPoint(_("pos")),
			obj->GetPropertyAsSize(_("size")),
			obj->GetPropertyAsInteger(_("style")) | obj->GetPropertyAsInteger(_("window_style")),
			obj->GetPropertyAsInteger(_("min")),
			obj->GetPropertyAsInteger(_("max")),
			obj->GetPropertyAsInteger(_("initial")));
	}

	ticpp::Element* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxSpinCtrl"), obj->GetPropertyAsString(_("name")));
		xrc.AddWindowProperties();
		xrc.AddProperty(_("initial"),_("value"), XRC_TYPE_TEXT);
		xrc.AddProperty(_("min"),_("min"), XRC_TYPE_INTEGER);
		xrc.AddProperty(_("max"),_("max"), XRC_TYPE_INTEGER);
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc( ticpp::Element* xrcObj )
	{
		XrcToXfbFilter filter(xrcObj, _("wxSpinCtrl"));
		filter.AddWindowProperties();
		filter.AddProperty(_("value"),_("value"), XRC_TYPE_TEXT);
		filter.AddProperty(_("value"),_("initial"), XRC_TYPE_TEXT);
		filter.AddProperty(_("min"),_("min"), XRC_TYPE_INTEGER);
		filter.AddProperty(_("max"),_("max"), XRC_TYPE_INTEGER);

		return filter.GetXfbObject();
	}
};

class SpinButtonComponent : public ComponentBase
{
public:
	wxObject* Create(IObject *obj, wxObject *parent)
	{
		return new wxSpinButton((wxWindow *)parent,-1,
			obj->GetPropertyAsPoint(_("pos")),
			obj->GetPropertyAsSize(_("size")),
			obj->GetPropertyAsInteger(_("style")) | obj->GetPropertyAsInteger(_("window_style")));
	}

	ticpp::Element* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxSpinButton"), obj->GetPropertyAsString(_("name")));
		xrc.AddWindowProperties();
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc( ticpp::Element* xrcObj )
	{
		XrcToXfbFilter filter(xrcObj, _("wxSpinButton"));
		filter.AddWindowProperties();
		return filter.GetXfbObject();
	}
};

class CheckListBoxComponent : public ComponentBase
{
public:

	wxObject* Create(IObject *obj, wxObject *parent)
	{
		wxArrayString choices (obj->GetPropertyAsArrayString(_("choices")));
		wxCheckListBox *cl =
			new wxCheckListBox((wxWindow *)parent,-1,
			obj->GetPropertyAsPoint(_("pos")),
			obj->GetPropertyAsSize(_("size")),
			choices,
			obj->GetPropertyAsInteger(_("style")) | obj->GetPropertyAsInteger(_("window_style")));

		return cl;
	}

	ticpp::Element* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxCheckListBox"), obj->GetPropertyAsString(_("name")));
		xrc.AddWindowProperties();
		xrc.AddProperty(_("choices"), _("content"), XRC_TYPE_STRINGLIST);
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc( ticpp::Element* xrcObj )
	{
		XrcToXfbFilter filter(xrcObj, _("wxCheckListBox"));
		filter.AddWindowProperties();
		filter.AddProperty(_("content"), _("choices"), XRC_TYPE_STRINGLIST);
		return filter.GetXfbObject();
	}
};

class GridComponent : public ComponentBase
{
public:

	wxObject* Create(IObject *obj, wxObject *parent)
	{
		wxGrid *grid = new wxGrid((wxWindow *)parent,-1,
			obj->GetPropertyAsPoint(_("pos")),
			obj->GetPropertyAsSize(_("size")),
			obj->GetPropertyAsInteger(_("window_style")));

		grid->CreateGrid(
			obj->GetPropertyAsInteger(_("rows")),
			obj->GetPropertyAsInteger(_("cols")));

		grid->EnableDragColMove( obj->GetPropertyAsInteger( _("drag_col_move") ) != 0 );
		grid->EnableDragColSize( obj->GetPropertyAsInteger( _("drag_col_size") ) != 0 );
		grid->EnableDragGridSize( obj->GetPropertyAsInteger( _("drag_grid_size") ) != 0 );
		grid->EnableDragRowSize( obj->GetPropertyAsInteger( _("drag_row_size") ) != 0 );
		grid->EnableEditing( obj->GetPropertyAsInteger( _("editing") ) != 0 );
		grid->EnableGridLines( obj->GetPropertyAsInteger( _("grid_lines") ) != 0 );
		if ( !obj->IsNull( _("grid_line_color") ) )
		{
			grid->SetGridLineColour( obj->GetPropertyAsColour( _("grid_line_color") ) );
		}
		grid->SetMargins( obj->GetPropertyAsInteger( _("margin_width") ), obj->GetPropertyAsInteger( _("margin_height") ) );

		// Label Properties
		grid->SetColLabelAlignment( obj->GetPropertyAsInteger( _("col_label_horiz_alignment") ), obj->GetPropertyAsInteger( _("col_label_vert_alignment") ) );
		grid->SetColLabelSize( obj->GetPropertyAsInteger( _("col_label_size") ) );

		wxArrayString columnLabels = obj->GetPropertyAsArrayString( _("col_label_values") );
		for ( int i = 0; i < (int)columnLabels.size() && i < grid->GetNumberCols(); ++i )
		{
			grid->SetColLabelValue( i, columnLabels[i] );
		}

		wxArrayInt columnSizes = obj->GetPropertyAsArrayInt( _("column_sizes") );
		for ( int i = 0; i < (int)columnSizes.size() && i < grid->GetNumberCols(); ++i )
		{
			grid->SetColSize( i, columnSizes[i] );
		}

		grid->SetRowLabelAlignment( obj->GetPropertyAsInteger( _("row_label_horiz_alignment") ), obj->GetPropertyAsInteger( _("row_label_vert_alignment") ) );
		grid->SetRowLabelSize( obj->GetPropertyAsInteger( _("row_label_size") ) );

		wxArrayString rowLabels = obj->GetPropertyAsArrayString( _("row_label_values") );
		for ( int i = 0; i < (int)rowLabels.size() && i < grid->GetNumberRows(); ++i )
		{
			grid->SetRowLabelValue( i, rowLabels[i] );
		}

		wxArrayInt rowSizes = obj->GetPropertyAsArrayInt( _("row_sizes") );
		for ( int i = 0; i < (int)rowSizes.size() && i < grid->GetNumberRows(); ++i )
		{
			grid->SetRowSize( i, rowSizes[i] );
		}

		if ( !obj->IsNull( _("label_bg") ) )
		{
			grid->SetLabelBackgroundColour( obj->GetPropertyAsColour( _("label_bg") ) );
		}
		if ( !obj->IsNull( _("label_text") ) )
		{
			grid->SetLabelTextColour( obj->GetPropertyAsColour( _("label_text") ) );
		}
		if ( !obj->IsNull( _("label_font") ) )
		{
			grid->SetLabelFont( obj->GetPropertyAsFont( _("label_font") ) );
		}

		// Default Cell Properties
		grid->SetDefaultCellAlignment( obj->GetPropertyAsInteger( _("cell_horiz_alignment") ), obj->GetPropertyAsInteger( _("cell_vert_alignment") ) );

		if ( !obj->IsNull( _("cell_bg") ) )
		{
			grid->SetDefaultCellBackgroundColour( obj->GetPropertyAsColour( _("cell_bg") ) );
		}
		if ( !obj->IsNull( _("cell_text") ) )
		{
			grid->SetDefaultCellTextColour( obj->GetPropertyAsColour( _("cell_text") ) );
		}
		if ( !obj->IsNull( _("cell_font") ) )
		{
			grid->SetDefaultCellFont( obj->GetPropertyAsFont( _("cell_font") ) );
		}

		// Example Cell Values
		for ( int col = 0; col < grid->GetNumberCols(); ++col )
		{
			for ( int row = 0; row < grid->GetNumberRows(); ++row )
			{
				grid->SetCellValue( row, col, grid->GetColLabelValue( col ) + wxT("-") + grid->GetRowLabelValue( row ) );
			}
		}

		if ( obj->GetPropertyAsInteger( _("autosize_rows") ) != 0 )
		{
			grid->AutoSizeRows();
		}
		if ( obj->GetPropertyAsInteger( _("autosize_cols") ) != 0 )
		{
			grid->AutoSizeColumns();
		}

		grid->PushEventHandler( new ComponentEvtHandler( grid, GetManager() ) );

		return grid;
	}

	ticpp::Element* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxGrid"), obj->GetPropertyAsString(_("name")));
		xrc.AddWindowProperties();
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc( ticpp::Element* xrcObj )
	{
		XrcToXfbFilter filter(xrcObj, _("wxGrid"));
		filter.AddWindowProperties();
		return filter.GetXfbObject();
	}
};

void ComponentEvtHandler::OnGridClick( wxGridEvent& event )
{
	m_manager->SelectObject( m_window );
	event.Skip();
}

void ComponentEvtHandler::OnGridColSize( wxGridSizeEvent& )
{
	wxGrid* grid = wxDynamicCast( m_window, wxGrid );
	if ( NULL == grid )
	{
		return;
	}

	wxString sizes;
	for ( int i = 0; i < grid->GetNumberCols(); ++i )
	{
		sizes += wxString::Format( wxT("%i,"), grid->GetColSize( i ) );
	}
	sizes = sizes.substr( 0, sizes.length() - 1 );

	m_manager->ModifyProperty( m_window, _("column_sizes"), sizes, true );
}

void ComponentEvtHandler::OnGridRowSize( wxGridSizeEvent& )
{
	wxGrid* grid = wxDynamicCast( m_window, wxGrid );
	if ( NULL == grid )
	{
		return;
	}

	wxString sizes;
	for ( int i = 0; i < grid->GetNumberRows(); ++i )
	{
		sizes += wxString::Format( wxT("%i,"), grid->GetRowSize( i ) );
	}
	sizes = sizes.substr( 0, sizes.length() - 1 );

	m_manager->ModifyProperty( m_window, _("row_sizes"), sizes, true );
}

#if wxCHECK_VERSION( 2, 8, 0 )

class ColourPickerComponent : public ComponentBase
{
public:
	wxObject* Create(IObject *obj, wxObject *parent)
	{
		wxColourPickerCtrl* colourpicker = new wxColourPickerCtrl(
			(wxWindow*)parent,
			obj->GetPropertyAsInteger(_("id")),
			obj->GetPropertyAsColour(_("colour")),
			obj->GetPropertyAsPoint(_("pos")),
			obj->GetPropertyAsSize(_("size")),
			obj->GetPropertyAsInteger(_("style")) | obj->GetPropertyAsInteger(_("window_style"))
			);

		colourpicker->PushEventHandler( new ComponentEvtHandler( colourpicker, GetManager() ) );
		return colourpicker;
	}

	ticpp::Element* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxColourPickerCtrl"), obj->GetPropertyAsString(_("name")));
		xrc.AddProperty(_("colour"),_("value"),XRC_TYPE_COLOUR);
		xrc.AddWindowProperties();
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc( ticpp::Element* xrcObj )
	{
		XrcToXfbFilter filter(xrcObj, _("wxColourPickerCtrl"));
		filter.AddProperty(_("value"),_("colour"),XRC_TYPE_COLOUR);
		filter.AddWindowProperties();
		return filter.GetXfbObject();
	}
};

void ComponentEvtHandler::OnColourPickerColourChanged( wxColourPickerEvent& )
{
	wxColourPickerCtrl* window = wxDynamicCast( m_window, wxColourPickerCtrl );
	if ( window != NULL )
	{
		wxColour colour = window->GetColour();
		m_manager->ModifyProperty( window, _("colour"), wxString::Format(wxT("%d,%d,%d"),colour.Red(),colour.Green(),colour.Blue())  );
	}
}

class FontPickerComponent : public ComponentBase
{
public:
	wxObject* Create(IObject *obj, wxObject *parent)
	{
		wxFontPickerCtrl* picker = new wxFontPickerCtrl(
			(wxWindow*)parent,
			obj->GetPropertyAsInteger(_("id")),
			obj->GetPropertyAsFont(_("value")),
			obj->GetPropertyAsPoint(_("pos")),
			obj->GetPropertyAsSize(_("size")),
			obj->GetPropertyAsInteger(_("style")) | obj->GetPropertyAsInteger(_("window_style"))
			);

		if ( !obj->IsNull( _("max_point_size") ) )
		{
			picker->SetMaxPointSize( obj->GetPropertyAsInteger( _("max_point_size") ) );
		}

		picker->PushEventHandler( new ComponentEvtHandler( picker, GetManager() ) );
		return picker;
	}

	ticpp::Element* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxFontPickerCtrl"), obj->GetPropertyAsString(_("name")));
		if ( !obj->IsNull( _("value") ) )
		{
			xrc.AddProperty(_("value"),_("value"),XRC_TYPE_FONT);
		}
		xrc.AddWindowProperties();
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc( ticpp::Element* xrcObj )
	{
		XrcToXfbFilter filter(xrcObj, _("wxFontPickerCtrl"));
		filter.AddProperty(_("value"),_("value"),XRC_TYPE_FONT);
		filter.AddWindowProperties();
		return filter.GetXfbObject();
	}
};

void ComponentEvtHandler::OnFontPickerFontChanged( wxFontPickerEvent& )
{
	wxFontPickerCtrl* window = wxDynamicCast( m_window, wxFontPickerCtrl );
	if ( window != NULL )
	{
		wxFont font = window->GetSelectedFont();
		m_manager->ModifyProperty( window, _("value"), wxString::Format( wxT("%s,%d,%d,%d"), font.GetFaceName().c_str(), font.GetStyle(), font.GetWeight(), font.GetPointSize() ) );
	}
}

class FilePickerComponent : public ComponentBase
{
public:
	wxObject* Create(IObject *obj, wxObject *parent)
	{
		wxFilePickerCtrl* picker = new wxFilePickerCtrl(
			(wxWindow*)parent,
			obj->GetPropertyAsInteger(_("id")),
			obj->GetPropertyAsString(_("value")),
			obj->GetPropertyAsString(_("message")),
			obj->GetPropertyAsString(_("wildcard")),
			obj->GetPropertyAsPoint(_("pos")),
			obj->GetPropertyAsSize(_("size")),
			obj->GetPropertyAsInteger(_("style")) | obj->GetPropertyAsInteger(_("window_style"))
			);

		picker->PushEventHandler( new ComponentEvtHandler( picker, GetManager() ) );
		return picker;
	}

	ticpp::Element* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxFilePickerCtrl"), obj->GetPropertyAsString(_("name")));
		xrc.AddProperty(_("value"),_("value"),XRC_TYPE_TEXT);
		xrc.AddProperty(_("message"),_("message"),XRC_TYPE_TEXT);
		xrc.AddProperty(_("wildcard"),_("wildcard"),XRC_TYPE_TEXT);
		xrc.AddWindowProperties();
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc( ticpp::Element* xrcObj )
	{
		XrcToXfbFilter filter(xrcObj, _("wxFilePickerCtrl"));
		filter.AddProperty(_("value"),_("value"),XRC_TYPE_FONT);
		filter.AddProperty(_("message"),_("message"),XRC_TYPE_TEXT);
		filter.AddProperty(_("wildcard"),_("wildcard"),XRC_TYPE_TEXT);
		filter.AddWindowProperties();
		return filter.GetXfbObject();
	}
};

void ComponentEvtHandler::OnFilePickerFileChanged( wxFileDirPickerEvent& )
{
	wxFilePickerCtrl* window = wxDynamicCast( m_window, wxFilePickerCtrl );
	if ( window != NULL )
	{
		m_manager->ModifyProperty( window, _("value"), window->GetPath() );
	}
}

class DirPickerComponent : public ComponentBase
{
public:
	wxObject* Create(IObject *obj, wxObject *parent)
	{
		wxDirPickerCtrl* picker = new wxDirPickerCtrl(
			(wxWindow*)parent,
			obj->GetPropertyAsInteger(_("id")),
			obj->GetPropertyAsString(_("value")),
			obj->GetPropertyAsString(_("message")),
			obj->GetPropertyAsPoint(_("pos")),
			obj->GetPropertyAsSize(_("size")),
			obj->GetPropertyAsInteger(_("style")) | obj->GetPropertyAsInteger(_("window_style"))
			);

		picker->PushEventHandler( new ComponentEvtHandler( picker, GetManager() ) );
		return picker;
	}

	ticpp::Element* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxDirPickerCtrl"), obj->GetPropertyAsString(_("name")));
		xrc.AddProperty(_("value"),_("value"),XRC_TYPE_TEXT);
		xrc.AddProperty(_("message"),_("message"),XRC_TYPE_TEXT);
		xrc.AddWindowProperties();
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc( ticpp::Element* xrcObj )
	{
		XrcToXfbFilter filter(xrcObj, _("wxDirPickerCtrl"));
		filter.AddProperty(_("value"),_("value"),XRC_TYPE_FONT);
		filter.AddProperty(_("message"),_("message"),XRC_TYPE_TEXT);
		filter.AddWindowProperties();
		return filter.GetXfbObject();
	}
};

void ComponentEvtHandler::OnDirPickerDirChanged( wxFileDirPickerEvent& )
{
	wxDirPickerCtrl* window = wxDynamicCast( m_window, wxDirPickerCtrl );
	if ( window != NULL )
	{
		m_manager->ModifyProperty( window, _("value"), window->GetPath() );
	}
}

class HyperlinkComponent : public ComponentBase
{
public:
	wxObject* Create(IObject *obj, wxObject *parent)
	{
		wxHyperlinkCtrl* ctrl = new wxHyperlinkCtrl(
			(wxWindow*)parent, -1,
			obj->GetPropertyAsString(_("label")),
			obj->GetPropertyAsString(_("url")),
			obj->GetPropertyAsPoint(_("pos")),
			obj->GetPropertyAsSize(_("size")),
			obj->GetPropertyAsInteger(_("style")) | obj->GetPropertyAsInteger(_("window_style"))
			);

		if ( !obj->IsNull( _("hover_color") ) )
		{
			ctrl->SetHoverColour( obj->GetPropertyAsColour( _("hover_color") ) );
		}
		if ( !obj->IsNull( _("normal_color") ) )
		{
			ctrl->SetNormalColour( obj->GetPropertyAsColour( _("normal_color") ) );
		}
		if ( !obj->IsNull( _("visited_color") ) )
		{
			ctrl->SetVisitedColour( obj->GetPropertyAsColour( _("visited_color") ) );
		}

		return ctrl;
	}

	ticpp::Element* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxHyperlinkCtrl"), obj->GetPropertyAsString(_("name")));
		xrc.AddProperty(_("label"),_("label"),XRC_TYPE_TEXT);
		xrc.AddProperty(_("url"),_("url"),XRC_TYPE_TEXT);
		xrc.AddWindowProperties();
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc( ticpp::Element* xrcObj )
	{
		XrcToXfbFilter filter(xrcObj, _("wxHyperlinkCtrl"));
		filter.AddProperty(_("label"),_("label"),XRC_TYPE_TEXT);
		filter.AddProperty(_("url"),_("url"),XRC_TYPE_TEXT);
		filter.AddWindowProperties();
		return filter.GetXfbObject();
	}
};

#endif

class GenericDirCtrlComponent : public ComponentBase
{
public:
	wxObject* Create( IObject* obj, wxObject* parent )
	{
		wxGenericDirCtrl* ctrl = new wxGenericDirCtrl(
			(wxWindow*)parent,
			wxID_ANY,
			obj->GetPropertyAsString(_("defaultfolder")),
			obj->GetPropertyAsPoint(_("pos")),
			obj->GetPropertyAsSize(_("size")),
			obj->GetPropertyAsInteger(_("style")) | obj->GetPropertyAsInteger(_("window_style")),
			obj->GetPropertyAsString(_("filter")),
			obj->GetPropertyAsInteger(_("defaultfilter"))
			);

		ctrl->ShowHidden( obj->GetPropertyAsInteger( _("show_hidden") ) != 0 );
		ctrl->GetTreeCtrl()->PushEventHandler( new GenericDirCtrlEvtHandler( ctrl, GetManager() ) );
		return ctrl;
	}

	ticpp::Element* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, _("wxGenericDirCtrl"), obj->GetPropertyAsString(_("name")));
		xrc.AddProperty(_("defaultfolder"),_("defaultfolder"),XRC_TYPE_TEXT);
		xrc.AddProperty(_("filter"),_("filter"),XRC_TYPE_TEXT);
		xrc.AddProperty(_("defaultfilter"),_("defaultfilter"),XRC_TYPE_INTEGER);
		xrc.AddWindowProperties();
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc( ticpp::Element* xrcObj )
	{
		XrcToXfbFilter filter(xrcObj, _("wxGenericDirCtrl"));
		filter.AddProperty(_("defaultfolder"),_("defaultfolder"),XRC_TYPE_TEXT);
		filter.AddProperty(_("filter"),_("filter"),XRC_TYPE_TEXT);
		filter.AddProperty(_("defaultfilter"),_("defaultfilter"),XRC_TYPE_INTEGER);
		filter.AddWindowProperties();
		return filter.GetXfbObject();
	}
};

void GenericDirCtrlEvtHandler::OnGenericDirCtrlLeftClick( wxMouseEvent& event )
{
	m_manager->SelectObject( m_window );
	event.Skip();
}

class CustomControlComponent : public ComponentBase
{
public:
	wxObject* Create(IObject* /*obj*/, wxObject *parent)
	{
		return new wxPanel((wxWindow *)parent, -1, wxDefaultPosition, wxDefaultSize, 0 );
	}

	ticpp::Element* ExportToXrc(IObject *obj)
	{
		ObjectToXrcFilter xrc(obj, obj->GetPropertyAsString(_("class")), obj->GetPropertyAsString(_("name")));
		return xrc.GetXrcObject();
	}
};

///////////////////////////////////////////////////////////////////////////////

BEGIN_LIBRARY()

WINDOW_COMPONENT("wxCalendarCtrl",CalendarCtrlComponent)
WINDOW_COMPONENT("wxDatePickerCtrl", DatePickerCtrlComponent )
WINDOW_COMPONENT("wxHtmlWindow",HtmlWindowComponent)
WINDOW_COMPONENT("wxToggleButton",ToggleButtonComponent)
WINDOW_COMPONENT("wxTreeCtrl",TreeCtrlComponent)
WINDOW_COMPONENT("wxGrid",GridComponent)
WINDOW_COMPONENT("wxScrollBar",ScrollBarComponent)
WINDOW_COMPONENT("wxSpinCtrl",SpinCtrlComponent)
WINDOW_COMPONENT("wxSpinButton",SpinButtonComponent)
WINDOW_COMPONENT("CustomControl", CustomControlComponent)

// wxCheckListBox
WINDOW_COMPONENT("wxCheckListBox",CheckListBoxComponent)

#if wxCHECK_VERSION( 2, 8, 0 )
// wxRichTextCtrl
WINDOW_COMPONENT( "wxRichTextCtrl", RichTextCtrlComponent )
MACRO(wxTE_PROCESS_ENTER);
MACRO(wxTE_PROCESS_TAB);
MACRO(wxTE_READONLY);
MACRO(wxTE_AUTO_URL);

// wxColourPickerCtrl
WINDOW_COMPONENT("wxColourPickerCtrl", ColourPickerComponent)
MACRO(wxCLRP_DEFAULT_STYLE)
MACRO(wxCLRP_USE_TEXTCTRL)
MACRO(wxCLRP_SHOW_LABEL)

// wxFontPickerCtrl
WINDOW_COMPONENT("wxFontPickerCtrl", FontPickerComponent)
MACRO(wxFNTP_DEFAULT_STYLE)
MACRO(wxFNTP_USE_TEXTCTRL)
MACRO(wxFNTP_FONTDESC_AS_LABEL)
MACRO(wxFNTP_USEFONT_FOR_LABEL)

// wxFilePickerCtrl
WINDOW_COMPONENT("wxFilePickerCtrl", FilePickerComponent)
MACRO(wxFLP_DEFAULT_STYLE)
MACRO(wxFLP_USE_TEXTCTRL)
MACRO(wxFLP_OPEN)
MACRO(wxFLP_SAVE)
MACRO(wxFLP_OVERWRITE_PROMPT)
MACRO(wxFLP_FILE_MUST_EXIST)
MACRO(wxFLP_CHANGE_DIR)

// wxDirPickerCtrl
WINDOW_COMPONENT("wxDirPickerCtrl", DirPickerComponent)
MACRO(wxDIRP_DEFAULT_STYLE)
MACRO(wxDIRP_USE_TEXTCTRL)
MACRO(wxDIRP_DIR_MUST_EXIST)
MACRO(wxDIRP_CHANGE_DIR)

// wxHyperlinkCtrl
WINDOW_COMPONENT("wxHyperlinkCtrl", HyperlinkComponent)
MACRO(wxHL_ALIGN_LEFT)
MACRO(wxHL_ALIGN_RIGHT)
MACRO(wxHL_ALIGN_CENTRE)
MACRO(wxHL_CONTEXTMENU)
MACRO(wxHL_DEFAULT_STYLE)


#endif

// wxCalendarCtrl
MACRO(wxCAL_SUNDAY_FIRST)
MACRO(wxCAL_MONDAY_FIRST)
MACRO(wxCAL_SHOW_HOLIDAYS)
MACRO(wxCAL_NO_YEAR_CHANGE)
MACRO(wxCAL_NO_MONTH_CHANGE)
MACRO(wxCAL_SHOW_SURROUNDING_WEEKS)
MACRO(wxCAL_SEQUENTIAL_MONTH_SELECTION)

// wxDatePickerCtrl
MACRO(wxDP_SPIN)
MACRO(wxDP_DROPDOWN)
MACRO(wxDP_SHOWCENTURY)
MACRO(wxDP_ALLOWNONE)
MACRO(wxDP_DEFAULT)

// wxHtmlWindow
MACRO(wxHW_SCROLLBAR_NEVER)
MACRO(wxHW_SCROLLBAR_AUTO)
MACRO(wxHW_NO_SELECTION)

// wxTreeCtrl
MACRO(wxTR_EDIT_LABELS)
MACRO(wxTR_NO_BUTTONS)
MACRO(wxTR_HAS_BUTTONS)
MACRO(wxTR_TWIST_BUTTONS)
MACRO(wxTR_NO_LINES)
MACRO(wxTR_FULL_ROW_HIGHLIGHT)
MACRO(wxTR_LINES_AT_ROOT)
MACRO(wxTR_HIDE_ROOT)
MACRO(wxTR_ROW_LINES)
MACRO(wxTR_HAS_VARIABLE_ROW_HEIGHT)
MACRO(wxTR_SINGLE)
MACRO(wxTR_MULTIPLE)
MACRO(wxTR_EXTENDED)
MACRO(wxTR_DEFAULT_STYLE)

// wxGrid
MACRO(wxALIGN_LEFT)
MACRO(wxALIGN_CENTRE)
MACRO(wxALIGN_RIGHT)
MACRO(wxALIGN_TOP)
MACRO(wxALIGN_BOTTOM)

// wxScrollBar
MACRO(wxSB_HORIZONTAL)
MACRO(wxSB_VERTICAL)

// wxSpinCtrl and wxSpinButton
MACRO(wxSP_ARROW_KEYS)
MACRO(wxSP_WRAP)
MACRO(wxSP_HORIZONTAL)
MACRO(wxSP_VERTICAL)

// wxGenericDirCtrl
WINDOW_COMPONENT("wxGenericDirCtrl",GenericDirCtrlComponent)
MACRO(wxDIRCTRL_DIR_ONLY)
MACRO(wxDIRCTRL_3D_INTERNAL)
MACRO(wxDIRCTRL_SELECT_FIRST)
MACRO(wxDIRCTRL_SHOW_FILTERS)
MACRO(wxDIRCTRL_EDIT_LABELS)

END_LIBRARY()

