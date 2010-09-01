// CSdl.h  -  headaren fail na Sdl s opisaniq na funkciite na clasa



#ifndef __CSDL
#define __CSDL

////classes//////////////
class CGame;
class CSound;
class CKdf_Packeger;
/////////////////////////

#define RELEASE_SURF( surf ) if ( surf ) { SDL_FreeSurface( surf ); }

//#define FONT_TTF						// use TTF_FONTS

#define MAX_SPRITES  60 				// maximum kartinki na ekarana
#define SDL_FAIL     0	      			// flagove za greshki
#define SDL_OK       -1
#define NO_COLORKEY  -1
#define NO_ALPHA	 255
#define MAX_SOUNDS	 30					// maximum slotove za sounda

#define MOUSE_BUTTON_UNPRESSED	0
#define MOUSE_BUTTON_UP			1
#define MOUSE_BUTTON_DOWN		2
#define JOY_AXIS_UP			    1
#define JOY_AXIS_DOWN			2
#define JOY_AXIS_LEFT			3
#define JOY_AXIS_RIGHT			4
#define JOY_AXIS_NONE		    5

#define BLACK		   0x0
#define WHITE		   0xffffff
#define MAGENTA		   0xff00ff
#define MAGENTA_565	   0xF81F		
#define MAGENTA_555	   0x7C1F
#define SHADOW_MASK565 0xF7DE
#define SHADOW_MASK555 0xFBDE

#ifdef RGB_MODE555
 #define RGB_16BIT(r,g,b) ( ((r) << 10) | ((g) << 5) | (b) ) 
#else
 #define RGB_16BIT(r,g,b) ( ((r) << 11) | ((g) << 5) | (b) ) 
#endif

#define ANALOG_THRESHOLD 20000


typedef struct _BITMAPFILEHEADER {	// bmfh 
    Uint16  bfType; 
    Uint32  bfSize; 
    Uint16  bfReserved1; 
    Uint16  bfReserved2; 
    Uint32  bfOffBits; 
} __BITMAPFILEHEADER; 



// (SDL_Mixer) Sound Class
class CSound
{

public:

	CSound()
		: sound(NULL), buffered(false), play_channel(-1), loaded(false)
	{
	};

	~CSound()
	{
	};

	FSOUND_SAMPLE   *sound;
	bool			buffered;				// bufferiran li shte e zvukyt 
	int				play_channel;			// ne-bufferiran zvuk se nujdae ot otdelen kanal
	bool			loaded;
	
	void Release();

};



class CSdl 
{

protected:

	// graphics
	typedef struct STRUCT_BLIT 
	{
		int x,y,z;
		SDL_Surface *surf;
	};

	typedef std::vector<int>	udtButtonsBuffer;
   
	SDL_Surface   *screen;			    // glavna powyrhnost(buffer) na Sdl-to
	STRUCT_BLIT   surface[MAX_SPRITES];
    Uint32		  num_surfaces;			// broi kartinki koito da bydat rendirani pri sledwashitq kadyr  
	int			  mouse_x, mouse_y;
	int			  mouse_lbutton, mouse_rbutton;

	Uint16		  magenta;
	CGame		  *_game;	


#ifdef FONT_TTF
   TTF_Font       *font_ttf;
#else
   SDL_Surface    *font_bmp;
   Uint16		  font_size;
#endif

   CSound		  sounds[MAX_SOUNDS];	
   bool			  bsound_initialized;	 // flag-rezultat inicializaciqta na zvuka
   int			  volume_sound;
   int			  volume_music;

   SDL_Joystick		*_joystick;
   bool				_bJoystickSupport;
   int 				_nJoystickDevices;
   int				_nJoystickIdxDeviceToUse;
   Sint16			_xJoystick;
   Sint16			_yJoystick;
   udtButtonsBuffer	_JoystickButtons;
 
public:

	Uint8				*keys;				// masiv s aktivnite klavishi (natisnati)
	int					JoystickAxisX;
	int					JoystickAxisY;
	Uint8				JoystickHatState;
	

protected:
	void		_Blitall();
	Uint32		_GetPixel(SDL_Surface *surface, int x, int y);
	int			_Slock( SDL_Surface *surface );
	void		_Sunlock( SDL_Surface *surface );
	int			_ClipRect( int *x , int *y, SDL_Rect *rSurf );

//	SDL_Surface* LoadBitmap( char *filename, long file_offset, Uint32 file_size, Uint32 color_key = NO_COLORKEY, Uint16 alpha_value = NO_ALPHA);

public:
	CSdl();
	~CSdl();

	// sdl_gfx
	bool Initialize( CGame *game, int nWidth, int nHeight, int nBpp, int bFullscreen, int bHardware = false );
	bool InitializeJoystick();
	bool AcquireJoystick();
	void UnAcquireJoystick();
	void ReleaseJoystick();
	void Close();
	void Flip();
	void FlipTo( SDL_Surface *dest_surf );
	int  Addtoblit( int x, int y, SDL_Surface *surf );
	void BlitNow( int x, int y, SDL_Surface *surf );
	void BlitNow( int x, int y, SDL_Surface *surf, SDL_Rect *rsurf );
	void BlitShadow( int x, int y, int *mask, SDL_Rect *rsurf );
	void BlitShadow( int x, int y, SDL_Surface *surf);
	int  Collide( SDL_Rect *r_result, SDL_Rect *r1, SDL_Rect *r2 );
	int  Collide( SDL_Rect *r1, int *mask1, SDL_Rect *r2, int *mask2 );
	void MakeBoolMask( SDL_Surface *surf, int *& );
	//SDL_Surface* LoadBitmap( char *filename, Uint32 color_key = NO_COLORKEY, Uint8 alpha_value = NO_ALPHA );
	//SDL_Surface* LoadBitmap( char *filename, long file_offset, Uint32 file_size, Uint32 color_key = NO_COLORKEY, Uint8 alpha_value = NO_ALPHA );
	SDL_Surface* LoadBitmap( char *filename, Uint32 color_key = NO_COLORKEY, Uint8 alpha_value = NO_ALPHA );
	SDL_Surface* LoadBitmap( char *filename, long file_offset, Uint32 file_size, Uint32 color_key = NO_COLORKEY, Uint16 alpha_value = NO_ALPHA);
	SDL_Surface* CreateEmptySurface( int width, int height );
	void	SetRect( SDL_Rect *rect, int x, int y, int width, int height );

	// sdl_font
#ifdef FONT_TTF
	void InitializeFont( int fontsize = 12 );
	void DrawText( int x, int y, char *text, SDL_Color forecolor );
	void DrawText( int x, int y, char *text, SDL_Color forecolor, SDL_Color backcolor );
#else
	void InitializeFont();
    void DrawNum( int x, int y, char *text );
#endif

	// sdl_colors
	SDL_Color CreateColor( int r, int g, int b, int a );
	SDL_Color CreateColor( int r, int g, int b );
	/*bool operator==( SDL_Color clr1, SDL_Color clr2 ) { return clr1.r == clr2.r && clr1.g == clr2.g && clr1.b == clr2.b; }
	bool operator!=( SDL_Color clr1, SDL_Color clr2 ) { return clr1.r != clr2.r || clr1.g != clr2.g || clr1.b != clr2.b; }
    */

	// sdl input
	void GetInput();					// za klavishite
	bool GetJoystickButtonPressed( int idx );
	int	 GetMouseX() { return mouse_x; };
	int  GetMouseY() { return mouse_y; };
	int	 GetMouseLButton() { return mouse_lbutton; };
	int	 GetMouseRButton() { return mouse_rbutton; };


	// Sound functions
	int  LoadSound( char *filename, bool buffered_sound );
	void PlaySound( int snd_index, int position = -1 );
	//Mix_Chunk* LoadWav( char *filename, long file_offset, Uint32 file_size );
	//void PlaySound( int snd_index );

	int  GetSoundVolume() { return volume_sound; };
	int  GetMusicVolume() { return volume_music; };
	void SetSoundVolume( int );
	void SetMusicVolume( int );
	void ChangeSoundVolume( int s_vol ) { volume_sound += s_vol; SetSoundVolume( volume_sound ); };
	void ChangeMusicVolume( int m_vol ) { volume_music += m_vol; SetMusicVolume( volume_music ); };

};





#endif