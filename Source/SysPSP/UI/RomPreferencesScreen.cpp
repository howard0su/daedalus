/*
Copyright (C) 2006 StrmnNrmn

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/


#include "Base/Types.h"


#include <stdio.h>

#include <pspctrl.h>

#include "Config/ConfigOptions.h"
#include "Core/ROM.h"
#include "Core/RomSettings.h"
#include "Graphics/ColourValue.h"
#include "Input/InputManager.h"
#include "SysPSP/Graphics/DrawText.h"
#include "SysPSP/UI/PSPMenu.h"
#include "SysPSP/UI/RomPreferencesScreen.h"
#include "SysPSP/UI/UIContext.h"
#include "SysPSP/UI/UIScreen.h"
#include "SysPSP/UI/UISetting.h"
#include "SysPSP/UI/UISpacer.h"
#include "SysPSP/UI/UICommand.h"
#include "Interface/Preferences.h"


namespace
{

	class CTextureHashFrequency : public CUISetting
	{
	public:
		CTextureHashFrequency( ETextureHashFrequency * setting, const char * name, const char * description )
			:	CUISetting( name, description )
			,	mSetting( setting )
		{
		}

		virtual	void			OnNext()				{ *mSetting = ETextureHashFrequency( ( *mSetting + 1 ) % NUM_THF ); }
		virtual	void			OnPrevious()			{ *mSetting = ETextureHashFrequency( ( *mSetting + NUM_THF - 1 ) % NUM_THF ); }

		virtual const char *	GetSettingName() const	{ return Preferences_GetTextureHashFrequencyDescription( *mSetting ); }

	private:
		ETextureHashFrequency *	mSetting;
	};



	class CAdjustControllerSetting : public CUISetting
	{
	public:
		CAdjustControllerSetting( u32 * setting, const char * name )
			:	CUISetting( name, "" )
			,	mSetting( setting )
		{
		}

		virtual	void			OnNext()				{ *mSetting = (*mSetting + 1) % CInputManager::Get()->GetNumConfigurations(); }
		virtual	void			OnPrevious()			{ *mSetting = (*mSetting + CInputManager::Get()->GetNumConfigurations() - 1) % CInputManager::Get()->GetNumConfigurations(); }

		virtual const char *	GetSettingName() const	{ return CInputManager::Get()->GetConfigurationName( *mSetting ); }
		virtual const char *	GetDescription() const	{ return CInputManager::Get()->GetConfigurationDescription( *mSetting ); }

	private:
		u32 *					mSetting;
	};


	class CAudioSetting : public CUISetting
	{
	public:
		CAudioSetting( EAudioPluginMode * setting, const char * name, const char * description )
			:	CUISetting( name, description )
			,	mSetting( setting )
		{
		}

		virtual	void			OnNext()				{ *mSetting = (*mSetting < APM_ENABLED_SYNC) ? static_cast<EAudioPluginMode>(*mSetting + 1) : APM_DISABLED; }
		virtual	void			OnPrevious()			{ *mSetting = (*mSetting > APM_DISABLED)     ? static_cast<EAudioPluginMode>(*mSetting - 1) : APM_ENABLED_SYNC; }

		virtual const char *	GetSettingName() const
		{
			switch ( *mSetting )
			{
				case APM_DISABLED:			return "Disabled";
				case APM_ENABLED_ASYNC:		return "Asynchronous";
				case APM_ENABLED_SYNC:		return "Synchronous";
			}
			return "Unknown";
		}

	private:
		EAudioPluginMode		*mSetting;
	};

	class CZoomSetting : public CUISetting
	{
	public:
		CZoomSetting( f32 * setting, const char * name, const char * description )
			:	CUISetting( name, description )
			,	mSetting( setting )
		{
		}

		virtual	void			OnNext()				{ *mSetting += 0.01f; *mSetting = *mSetting > 1.5f ? 1.5f : *mSetting;}
		virtual	void			OnPrevious()			{ *mSetting -= 0.01f; *mSetting = *mSetting < 1.0f ? 1.0f : *mSetting;}

		virtual const char *	GetSettingName() const	{ snprintf( mString, sizeof(mString), "%.0f%%", (double)(*mSetting*100.0f) ); return mString; }

	private:
		float *			mSetting;
		mutable char 	mString[8];
	};

	class CAdjustFrequencySetting : public CUISetting
	{
	public:
		CAdjustFrequencySetting( bool * setting, EAudioPluginMode * audio_enabled, const char * name, const char * description )
			:	CUISetting( name, description )
			,	mSetting( setting )
			,	mAudioEnabled( audio_enabled )
		{
		}

		virtual bool			IsReadOnly() const		{ return *mAudioEnabled > APM_DISABLED; }		// Disable this if no audio enabled;

		virtual	void			OnNext()				{ if( !IsReadOnly() ) *mSetting = !*mSetting; }
		virtual	void			OnPrevious()			{ if( !IsReadOnly() ) *mSetting = !*mSetting; }

		virtual const char *	GetSettingName() const	{ return (*mSetting) ? "Enabled" : "Disabled"; }

	private:
		bool *					mSetting;
		EAudioPluginMode *		mAudioEnabled;
	};


	class CAdjustFrameskipSetting : public CUISetting
	{
	public:
		CAdjustFrameskipSetting( EFrameskipValue * setting, const char * name, const char * description )
			:	CUISetting( name, description )
			,	mSetting( setting )
		{
		}

		virtual	void			OnNext()				{ *mSetting = EFrameskipValue( (*mSetting + 1) % NUM_FRAMESKIP_VALUES ); }
		virtual	void			OnPrevious()			{ *mSetting = EFrameskipValue( (*mSetting + NUM_FRAMESKIP_VALUES - 1) % NUM_FRAMESKIP_VALUES ); }

		virtual const char *	GetSettingName() const	{ return Preferences_GetFrameskipDescription( *mSetting ); }

	private:
		EFrameskipValue *		mSetting;
	};

	class CFSkipSetting : public CUISetting
	{
	public:
		CFSkipSetting( u32 * setting, const char * name, const char * description )
			:	CUISetting( name, description )
			,	mSetting( setting )
		{
		}

		virtual	void			OnNext()				{ *mSetting = (*mSetting < 2) ? (*mSetting + 1) : 0; }
		virtual	void			OnPrevious()			{ *mSetting = (*mSetting > 0) ? (*mSetting - 1) : 2; }

		virtual const char *	GetSettingName() const
		{
			switch ( *mSetting )
			{
				case 0:		return "No";
				case 1:		return "Full speed";
				case 2:		return "Half speed";
			}
			return "?";
		}

	private:
		u32		*mSetting;
	};
}


//

class IRomPreferencesScreen : public CRomPreferencesScreen, public CUIScreen
{
	public:

		IRomPreferencesScreen( CUIContext * p_context, const RomID & rom_id );
		~IRomPreferencesScreen();

		// CRomPreferencesScreen
		virtual void				Run();

		// CUIScreen
		virtual void				Update( float elapsed_time, const v2 & stick, u32 old_buttons, u32 new_buttons );
		virtual void				Render();
		virtual bool				IsFinished() const									{ return mIsFinished; }

	private:
				void				OnConfirm();
				void				OnCancel();
				void				ResetDefaults();

	private:
		RomID						mRomID;
		std::string					mRomName;
		SRomPreferences				mRomPreferences;

		bool						mIsFinished;

		CUIElementBag				mElements;
};


//

CRomPreferencesScreen::~CRomPreferencesScreen()
{
}


//

CRomPreferencesScreen *	CRomPreferencesScreen::Create( CUIContext * p_context, const RomID & rom_id )
{
	return new IRomPreferencesScreen( p_context, rom_id );
}


IRomPreferencesScreen::IRomPreferencesScreen( CUIContext * p_context, const RomID & rom_id )
:	CUIScreen( p_context )
,	mRomID( rom_id )
,	mRomName( "?" )
,	mIsFinished( false )
{
	CPreferences::Get()->GetRomPreferences( mRomID, &mRomPreferences );

	RomSettings			settings;
	if ( CRomSettingsDB::Get()->GetSettings( rom_id, &settings ) )
	{
 		mRomName = settings.GameName;
	}

	mElements.Add( new CTextureHashFrequency( &mRomPreferences.CheckTextureHashFrequency, "Texture Update Check",	"Whether to check for texture updates between frames. Disable this to improve framerate at the expense of graphics quality in some ROMs." ) );
	mElements.Add( new CAdjustFrameskipSetting( &mRomPreferences.Frameskip, "Frameskip", "This determines how many frames are skipped before rendering a new frame. Increasing this value should give a small speedup, at the expense of more jerky graphics." ) );
	mElements.Add( new CZoomSetting( &mRomPreferences.ZoomX, "Zoom", "Increase screen size, the value will override the default screen size, 100% is default." ) );
	mElements.Add( new CFSkipSetting( &mRomPreferences.SpeedSyncEnabled, "Limit Framerate", "Limit the refresh rate to 50/25Hz (PAL) or 60/30Hz (NTSC)." ) );
	mElements.Add( new CBoolSetting( &mRomPreferences.DynarecEnabled, "Dynamic Recompilation", "Dynamic recompilation gives a considerable speed-up for the ROM emulation.", "Enabled", "Disabled" ) );
	mElements.Add( new CBoolSetting( &mRomPreferences.PatchesEnabled, "High Level Emulation", "Whether to use replicated OS function calls (faster) instead of emulating the real ones (slower) (WARNING, can cause instability and/or crash on certain ROMs).", "Enabled", "Disabled" ) );
	mElements.Add( new CAudioSetting( &mRomPreferences.AudioEnabled, "Audio", "Whether or not to enable audio emulation, and whether to process the audio asynchronously(fast/buggy) or synchronously(slow)." ) );
	mElements.Add( new CAdjustControllerSetting( &mRomPreferences.ControllerIndex, "Controller" ) );

//	mElements.Add( new CUISpacer( 16 ) );

	mElements.Add( new CUICommandImpl( new CMemberFunctor< IRomPreferencesScreen >( this, &IRomPreferencesScreen::OnConfirm ), "Save & Return", "Confirm changes to settings and return." ) );
	mElements.Add( new CUICommandImpl( new CMemberFunctor< IRomPreferencesScreen >( this, &IRomPreferencesScreen::OnCancel ), "Cancel", "Cancel changes to settings and return." ) );

}


//

IRomPreferencesScreen::~IRomPreferencesScreen() {}


//

void	IRomPreferencesScreen::Update( float elapsed_time, const v2 & stick, u32 old_buttons, u32 new_buttons )
{
	if(old_buttons != new_buttons)
	{
		if( new_buttons & PSP_CTRL_UP )
		{
			mElements.SelectPrevious();
		}
		if( new_buttons & PSP_CTRL_DOWN )
		{
			mElements.SelectNext();
		}

		CUIElement *	element( mElements.GetSelectedElement() );
		if( element != NULL )
		{
			if( new_buttons & PSP_CTRL_LEFT )
			{
				element->OnPrevious();
			}
			if( new_buttons & PSP_CTRL_RIGHT )
			{
				element->OnNext();
			}
			if( new_buttons & (PSP_CTRL_CROSS/*|PSP_CTRL_START*/) )
			{
				element->OnSelected();
			}
		}
	}
}


//

void	IRomPreferencesScreen::Render()
{
	mpContext->ClearBackground();

	u32		font_height( mpContext->GetFontHeight() );
	u32		line_height( font_height + 2 );
	s32		y;

	const char * const title_text = "Rom Preferences";
	mpContext->SetFontStyle( CUIContext::FS_HEADING );
	u32		heading_height( mpContext->GetFontHeight() );
	y = MENU_TOP + heading_height;
	mpContext->DrawTextAlign( LIST_TEXT_LEFT, LIST_TEXT_WIDTH, AT_CENTRE, y, title_text, mpContext->GetDefaultTextColour() ); y += heading_height;
	mpContext->SetFontStyle( CUIContext::FS_REGULAR );

	y += 2;

	mpContext->DrawTextAlign( LIST_TEXT_LEFT, LIST_TEXT_WIDTH, AT_CENTRE, y, mRomName.c_str(), mpContext->GetDefaultTextColour() ); y += line_height;

	y += 4;

	mElements.Draw( mpContext, LIST_TEXT_LEFT, LIST_TEXT_WIDTH, AT_CENTRE, y );

	CUIElement *	element( mElements.GetSelectedElement() );
	if( element != NULL )
	{
		const char *		p_description( element->GetDescription() );

		mpContext->DrawTextArea( DESCRIPTION_AREA_LEFT,
								 DESCRIPTION_AREA_TOP,
								 DESCRIPTION_AREA_RIGHT - DESCRIPTION_AREA_LEFT,
								 DESCRIPTION_AREA_BOTTOM - DESCRIPTION_AREA_TOP,
								 p_description,
								 DrawTextUtilities::TextWhite,
								 VA_BOTTOM );
	}
}


//

void	IRomPreferencesScreen::Run()
{
	CUIScreen::Run();
}


//

void	IRomPreferencesScreen::OnConfirm()
{
	CPreferences::Get()->SetRomPreferences( mRomID, mRomPreferences );

	CPreferences::Get()->Commit();

	mRomPreferences.Apply();

	mIsFinished = true;
}


//

void	IRomPreferencesScreen::OnCancel()
{
	mIsFinished = true;
}
