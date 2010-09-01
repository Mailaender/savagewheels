// Cswm_module.cpp - 


#include "Main.h"
#ifdef LINUX_BUILD
#	include <dirent.h>
#	include <sys/stat.h>
#else
#	include <io.h>
#endif


///////////////////////////////////////////////////////////////////////////////
//// Name: Cswv_modile()
//// Desc: class-constructor
///////////////////////////////////////////////////////////////////////////////
CSwv_module::CSwv_module()
:	num_vehicles( 0 ),
	pos_face( -1L )
{

}


///////////////////////////////////////////////////////////////////////////////
//// Name: ~Cswv_modile()
//// Desc: class-destructor
///////////////////////////////////////////////////////////////////////////////
CSwv_module::~CSwv_module()
{


}


///////////////////////////////////////////////////////////////////////////////
//// Name: Release()
//// Desc: free loaded modules memory
///////////////////////////////////////////////////////////////////////////////
void CSwv_module::Release()
{

	if ( num_vehicles != 0 )
	{
		for ( unsigned int i = 0; i < num_vehicles; i++ )
			delete[] vehicles[i].pfiles;

		delete[] vehicles;
	}

}



///////////////////////////////////////////////////////////////////////////////
//// Name: Create()
//// Desc: create a savagewheels-vehicles file from a header
///////////////////////////////////////////////////////////////////////////////
int CSwv_module::Create( SWV_HEADER *swm )
{

	FILE *fp;
	FILE *fp_bmp;
	char header[3] = { 'S', 'W', 'V' };
	char *buffer;
	long file_pos;
	int  num_anims;


	if ( ( fp = fopen( swm->filename, "wb" ) ) == NULL )
		return SWVERROR_OPENSWVFILE;

	// write headers
	strcpy( swm->header, header );
	//fwrite( header, sizeof(header)*3, 1, fp );
	fwrite( swm, sizeof(SWV_HEADER), 1, fp );

	// get number of animations ( +2 face.bmp dface.bmp )
	num_anims = swm->animation_frames * 2 + 3;
	
	file_pos = ftell( fp );
	fwrite( swm->pfiles, sizeof(SWV_FILES) * (num_anims), 1, fp ); 
	

	// put in bitmap data
	for ( int i = 0; i < num_anims; i++ )
	{
		// open bmp file
		if ( ( fp_bmp = fopen( swm->pfiles[i].filename, "rb" ) ) == NULL )
			return SWVERROR_OPENGFXFILE;

		// save position info in mother file
		swm->pfiles[i].pos = ftell( fp );
		
		// allocate memory & read its contents
		buffer = new char[swm->pfiles[i].length];
		fread( buffer, swm->pfiles[i].length, 1, fp_bmp );

		// send data to SWV_m
		fwrite( buffer, swm->pfiles[i].length, 1, fp );

		delete[] buffer;

		fclose( fp_bmp );

	}

	// write back file positions
	fseek( fp, file_pos, SEEK_SET );
	fwrite( swm->pfiles, sizeof(SWV_FILES) * (num_anims), 1, fp ); 


	fclose( fp );

	return SWV_SUCCESS;

}




///////////////////////////////////////////////////////////////////////////////
//// Name: Load()
//// Desc: loads a SWV file into the vehicles array
///////////////////////////////////////////////////////////////////////////////
int CSwv_module::Load( char *filename, SWV_HEADER *swv_file )
{


	FILE *fp;
	//FILE *fp_bmp;
	//char *buffer;
	//char header[3];
	int  num_anims;
	//char kbd;
	
	
	if ( ( fp = fopen( filename, "rb" ) ) == NULL )
		return SWVERROR_OPENSWVFILE;

	// read header
	fread( swv_file, sizeof(SWV_HEADER), 1, fp );

	if ( swv_file->header[0] != 'S' && swv_file->header[1] != 'W' && swv_file->header[2] != 'V' ) 
	{
		//...
		return SWVERROR_INCORRECTHEADER;
	}


	// read file_positions
	num_anims = swv_file->animation_frames * 2 + 3; // (+3 face.bmp dface.bmp, name.bmp)
	swv_file->pfiles = new SWV_FILES[num_anims];
	fread( swv_file->pfiles, sizeof(SWV_FILES) * (num_anims), 1, fp ); 

	/*for ( int i = 0; i < num_anims; i ++ )
	{
		
		cout << "\n file:" << vehicles->pfiles[i].filename;
		cout << "\n  len:" << vehicles->pfiles[i].length;
		cout << "\n  pos:" << vehicles->pfiles[i].pos;
		//if ( ((i+1)%24) == 0 ) cin >> kbd;
		//cin >> kbd;
	}*/
	
	//swv_file->pfiles[0].pos = swv_file->pfiles[0].pos;
	//swv_file->pfiles[0].length = swv_file->pfiles[0].length;


	// copy module_filename
	sprintf( swv_file->filename, "%s", filename );
	

	fclose( fp );

	return SWV_SUCCESS;
}



///////////////////////////////////////////////////////////////////////////////
//// Name: SearchAndLoad()
//// Desc: scans a folder for SWV files and loads them all
///////////////////////////////////////////////////////////////////////////////
int CSwv_module::SearchAndLoad( char *search_dir )
{

#ifdef LINUX_BUILD
	struct stat file_stat;
	struct dirent *pDirEntry = NULL;
	DIR	*pDir = NULL;	
#else
	struct _finddata_t  ffile;
	long   hFile;
#endif

	char   buf[255];
	
	// clear array from previous loadings
	if ( num_vehicles != 0 )
		delete[] vehicles;
		
	vehicles = new SWV_HEADER[16];

#ifndef LINUX_BUILD
	sprintf( buf, "%s/%s", search_dir, "*.swv" );

	if ( ( hFile = _findfirst( buf, &ffile ) ) != -1L )
#else
	pDir = opendir( search_dir );
	if ( pDir != NULL )
#endif
	{

#ifndef LINUX_BUILD
		sprintf( buf, "%s/%s", search_dir, ffile.name );

		//AppendToMultiLog( "Loading ...", buf, NULL );

		if ( Load( buf, &vehicles[num_vehicles] ) != SWV_SUCCESS )
			return SWVERROR_OPENSWVFILE;

		num_vehicles++;
		

		while ( _findnext( hFile, &ffile ) == 0 )
		{
			sprintf( buf, "%s/%s", search_dir, ffile.name );
			if ( Load( buf, &vehicles[num_vehicles] ) == SWV_SUCCESS )
				num_vehicles++;
		}
#else

		pDirEntry = readdir( pDir );
		while( pDirEntry != NULL )
		{
			if ( strcmp( pDirEntry->d_name, "." ) !=0 && strcmp( pDirEntry->d_name, ".." ) != 0 )
			{
				sprintf( buf, "%s/%s", search_dir, pDirEntry->d_name );

				#ifdef __DEBUG
					LOG( "[SWV] loading: " << buf );
				#endif
				
				if ( 0 == stat( buf, &file_stat ) )
				{
					if ( ! S_ISDIR( file_stat.st_mode ) )
					{
						sprintf( buf, "%s/%s", search_dir, pDirEntry->d_name );
						if ( Load( buf, &vehicles[num_vehicles] ) == SWV_SUCCESS )
							num_vehicles++;
					}
				}
			}

			pDirEntry = readdir( pDir );
		}
#ifdef __DEBUG
	sprintf( buf, "%d", num_vehicles );
	LOG( "[SWV] found modules=" << buf );
#endif

#endif

	}
	else
	{
		return SWVERROR_NOFILES;
	}

	//delete vehicles;

	return SWV_SUCCESS;
}