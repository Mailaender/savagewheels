// CMainMenu.h -


#ifndef __CMAINMENU
#define __CMAINMENU


////classes//////////////
class CGame;
class CSdl;
class CButton;
class CMainMenu;
/////////////////////////

#define NUM_BUTTONS_SELECT		8

#define NUM_BUTTONS_MENU		5
#define NUM_BUTTONS_OPTIONS		14 //12
#define NUM_BUTTONS_PLAY		1
#define NUM_BUTTONS_EXIT		1

#define MENU_OPTIONS			7//6
#define MENU_TEXTHEIGHT			20
#define MENU_TEXTWIDTH			190

#define MENU_BACKGROUNDS		1
#define MENU_BUTTONS			10

#define BUTTON_RIGHTC_UP		0
#define BUTTON_RIGHTC_DOWN		1
#define BUTTON_LEFTC_UP			2
#define BUTTON_LEFTC_DOWN		3
#define BUTTON_BACK_UP			4
#define BUTTON_BACK_DOWN   		5
#define BUTTON_OK_UP	 		6
#define BUTTON_OK_DOWN   		7
#define BUTTON_BUY_UP    		8
#define BUTTON_BUY_DOWN  		9

#define ANIM_ENGINE				0
#define ANIM_NOS   				1
#define ANIM_ARMOUR				2
#define ANIM_DAMAGE				3

#define STRINGS_SURFACES		4
#define STRINGS_SURFACE_MENU	0
#define STRINGS_SURFACE_OPTIONS	1
#define STRINGS_SURFACE_ONOFF	2




// rejimi na menu-to
typedef enum CONST_MENUSTATE
{
	MS_STARTSCREEN,
	MS_PLAY_SETUPCAR,
	MS_PLAY_1PLAYER,
	MS_PLAY_2PLAYER,
	MS_SETTINGS,
	//MS_INSTRUCTIONS,
	MS_EXIT,
	MS_CAMPAIGN_SETUP,	
	MS_CAMPAIGN_PREVIEW,
	MS_CAMPAIGN_WIN
};


typedef enum CONST_BUTTONTYPE
{
	BT_TEXT,
	BT_NAVIGATION
};


typedef enum CONST_BUTTONSTATE
{
	BS_NORMAL,
	BS_MOUSEOVER,
	BS_DOWN,		
	BS_UP
};

typedef enum CONST_BUTTONTEXT
{
	BTXT_MENU = 0,
	BTXT_OPTIONS = 1,
	BTXT_ONOFF = 2
};



// Button class
class CButton
{

private:
	CONST_BUTTONSTATE	state;
	CONST_BUTTONTYPE	type;
	int   				x, y;
	int   				tx, ty;
	Uint16				width, height;
	Uint32				pic_index;
	Uint32				text_pic_index;
	Uint32				text_index;
	bool				over_sound;
	

public:

	void Initialize( POINT *pos, Uint32 picture_index );					// button - navigaciq
	void Initialize( POINT *pos, Uint32 picture_index, Uint32 txt_index, Uint32 txtpicture_index );	// button - text
	void Update( CGame *game );
	
	CONST_BUTTONSTATE GetState() { return state; };

};



// Main menu class
class CMainMenu
{

private:

	CGame			  *_game;	

	SDL_Surface		  *ssBack[MENU_BACKGROUNDS];
	SDL_Surface		  **ssCarFace;
	SDL_Surface       **ssDriverFace;
	SDL_Surface		  **ssDriverName;
	SDL_Surface		  *ssCarAttribs;
	SDL_Surface		  *ssMenu[2];
	SDL_Surface		  *ssTitle[10];	
	SDL_Surface		  *ssUpg[4][35];
	SDL_Surface		  *savage_select[2];

	SDL_Surface		  *ssVolume;
	SDL_Surface		  *ssCursor[17];

	// campagin pix
	SDL_Surface		  *camp_select;
	SDL_Surface		  *camp_selquad;
	SDL_Surface		  *camp_preview;
	SDL_Surface	      *credits;

	CONST_MENUSTATE   Menustate;

	CButton			  buttons_menu[NUM_BUTTONS_MENU];
	CButton			  buttons_options[NUM_BUTTONS_OPTIONS + 1];
	CButton			  buttons_select[NUM_BUTTONS_SELECT];

	//CButton			  buttons_camp[2];
	int m,d;
	//CButton			  buttons_play[NUM_BUTTONS_PLAY];
	//CButton			  buttons_exit;


private:
	void SaveSettings();
	//void LoadSettings();
	int  ReloadCarsAttribs();
	void ReleaseCarsAttribs();
	void PreviewCar( int car_index );
	void ScrollScreen();
	void DoTitle();
	void UpdateCredits();

	bool LoadPlayerLevel( int );
	void ReleasePlayerLevel() { RELEASE_SURF( camp_preview ); camp_preview = NULL; };
	
public:

	CMainMenu();
	~CMainMenu();

	int  Initialize( CGame *game );
	void Update();
	void Release();

	/* put utside because music volume settings 
	wouldn't work before playing music!! */ 
	void LoadSettings();

};



#endif