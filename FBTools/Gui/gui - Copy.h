#ifndef GUI_H
#define GUI_H

#include <Windows.h>
#include <kernwin.hpp>
#include <vector>

int		widths[]    = { 18, 16, 16 };
char*	headers_1[] = { "Module Name", "Address", "Available Types" };
char*	headers_2[] = { "Type Name", "Address", "Size" };
char*	formats[]   = { "%d", "0x%x", "%d" };
int		num_cols   = 3;

char**	g_headers;
char**  g_chooserNames;
int*	g_dataValues_1;
int*	g_dataValues_2;

ulong idaapi idabook_sizer( void* obj )
{
	int* p = ( int* )obj;
	int count = 0;
	
	while ( *p++ ) count++;
	
	return count;
}

void idaapi idabook_getline( void* obj, ulong n, char* const* cells )
{
	int* p = ( int* )obj;

	msg( "%d \n", n );

	if ( n == 0 )
	{
		for ( int i = 0; i < num_cols; i++ )
		{
			qstrncpy( cells[i], g_headers[i], widths[i] );
		}
	} else {
		for ( int i = 0; i < num_cols; i++ )
		{
			if ( g_dataValues_2 == NULL && g_dataValues_1 == NULL )
				return;

			if ( i == 0 )
				qsnprintf( cells[i], widths[i], "%s", g_chooserNames[n] );
			else if ( i == 2 )
				qsnprintf( cells[i], widths[i], formats[i], g_dataValues_2[n] );
			else
				qsnprintf( cells[i], widths[i], formats[i], g_dataValues_1[n] ); //p[n - 1] );
		}
	}
}

class ChooseHelper
{
public:

	class Record
	{
	public:
		const char* name;
		int value_1;
		int value_2;
	};

	std::vector<Record*> m_records;

	ChooseHelper( )
	{
	};

	void add_record( LPCSTR title, int value_1, int value_2 )
	{
		Record* pRecord = new Record( );
		pRecord->name = title;
		pRecord->value_1 = value_1;
		pRecord->value_2 = value_2;

		m_records.push_back( pRecord );
	}

	void destroy_all( )
	{
		//ZeroMemory( g_dataValues_1, m_records.size( ) * sizeof( char* ) );
		//ZeroMemory( g_dataValues_2, m_records.size( ) * sizeof( char* ) );
		//TODO::add choser names

		m_records.clear( );

		free( g_chooserNames );
		free( g_dataValues_1 );
		free( g_dataValues_2 );
	}

	void export_to_ida( char** headers )
	{
		// Add last ending row
		this->add_record( "", 0, 0 );

		// Allocate necessary memory
		g_chooserNames = ( char** )malloc( m_records.size( ) * sizeof( char* ) );
		g_dataValues_1 = ( int*   )malloc( m_records.size( ) * sizeof( int*  ) );
		g_dataValues_2 = ( int*   )malloc( m_records.size( ) * sizeof( int*  ) );

		// Yeah.. yeah..
		g_headers = ( char** )malloc( num_cols * sizeof( char * ) );

		for ( int x = 0; x < num_cols; x++ )
		{
			g_headers[x] = headers[x];
		}

		// Copy from vector into this char** array for ida
		for ( unsigned int i = 0; i < m_records.size( ); i++ )
		{
			Record* pRecord = m_records.at( i );

			if ( pRecord )
			{
				if ( i == m_records.size( ) - 1 )
				{
					g_chooserNames[i] = NULL;
					g_dataValues_1[i] = 0;
					g_dataValues_2[i] = 0;
				} else {
					g_chooserNames[i] = ( char *)pRecord->name;
					g_dataValues_1[i] = pRecord->value_1;
					g_dataValues_2[i] = pRecord->value_2;
				}
			} else {
				msg( "[!] GUI Choose2 Error\n" );
			}
		}


		// convert our vector columns to the parrays ida needs

		//column_widths = ( PINT   )malloc( m_columns.size( ) * sizeof( int ) );
		//column_data	  = ( PINT   )malloc( m_columns.size( ) * sizeof( int ) );

		//msg( "size of data %d\n", sizeof( column_data ) );

		//column_headers = ( PCHAR* )malloc( m_columns.size( ) * sizeof( char* ) );
		//column_formats = ( PCHAR* )malloc( m_columns.size( ) * sizeof( char* ) );
	}

	~ChooseHelper( )
	{

	};
};

char* dialog = 
   "STARTITEM 0\n"
   "Find Classes\n\n"
   "Enter game module\n"
   "<String:A:32:32::>\n";
   
void idaapi button_handler( TView *fields[], int code )
{
   msg( "button was hit" );
}


/*


<#hint text#label:type:width:swidth:@hlp[]>

The individual components of a form field specifier are described in the
following list:
	#hint text# This element is optional. If present, the hint text, excluding
	the # characters, is displayed as a tool tip when the mouse hovers over the
	associated input field.
	label Static text displayed as a label to the left of the associated input
	field. In the case of button fields, this is the button text.
	type A single character indicates the type of form field being specified.
	Form field types are described following this list.
	width The maximum number of input characters accepted by the
	associated input field. In the case of button fields, this field specifies
	an integer button identification code used to distinguish one button
	from another.
	swidth The display width of the input field.
	@hlp[] This field is described in kernwin.hpp as “the number of help
	screen from the IDA.HLP file.” Since the content of this file is dictated
	by Hex-Rays, it seems unlikely that this field will be of use in the majority
	of cases. Substitute a colon for this field in order to ignore it.
	The characters used for the type field specify what type of input

Input field types va_list parameter
----------------- -----------------
A - ascii string char* at least MAXSTR size
S - segment sel_t*
N - hex number, C notation uval_t*
n - signed hex number, C notation sval_t*
L - default base (usually hex) number, ulonglong* C notation
l - default base (usually hex) number, longlong* signed C notation
M - hex number, no "0x" prefix uval_t*
D - decimal number sval_t*
O - octal number, C notation sval_t*
Y - binary number, "0b" prefix sval_t*
H - char value, C notation sval_t*
$ - address ea_t*
I - ident char* at least MAXNAMELEN size
B - button formcb_t button callback function
K - color button bgcolor_t*
C - checkbox ushort* bit mask of checked boxes
R - radiobutton ushort* number of selected radiobutton
*/

#endif





/*
ulong idaapi idabook_sizer( void* obj )
{
	int* p = ( int* )obj;
	int count = 0;
	
	while ( *p++ ) count++;
	
	return count;
}

void idaapi idabook_getline_2( void* obj, ulong n, char* const* cells )
{
	int *p = ( int* )obj;

	if (n == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			qstrncpy( cells[i], headers[i], widths[i] );
		}
	} else {
		for (int i = 0; i < 3; i++)
		{
			if ( i == 0 )
				qsnprintf( cells[i], widths[i], "%s", "Hello" );
			else
				qsnprintf( cells[i], widths[i], formats[i], p[n - 1] );
		}
	}
}

char* idaapi idabook_getline( void* obj, ulong n, char* buf )
{
	int* p = ( int* )obj;
	
	// Header
	if ( n == 0 )
	{
		qstrncpy( buf, "Value", strlen( "Value" ) + 1 );
	} else {
	// Data
		qsnprintf( buf, 32, "0x%08.8x", p[n-1] );
	}
	
	return buf;
}
*/