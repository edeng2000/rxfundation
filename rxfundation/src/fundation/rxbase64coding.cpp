#include "rxbase64coding.hpp"
#pragma hdrstop

#define CARRIAGE_RETURN (13)
#define LINE_FEED       (10)

#define END_OF_BASE64_ENCODED_DATA           ('=')
#define BASE64_END_OF_BUFFER                 (0xFD)
#define BASE64_IGNORABLE_CHARACTER           (0xFE)
#define BASE64_UNKNOWN_VALUE                 (0xFF)
#define BASE64_NUMBER_OF_CHARACTERS_PER_LINE (72)

static inline BYTE __get_character( const BYTE * buffer, const BYTE * decoder_table, int& index, int size_of_buffer )
{
   BYTE return_value = 0;

   do
   {
      if ( index >= size_of_buffer )
      {
         return( BASE64_END_OF_BUFFER );
      }

      return_value = buffer[ index ];
      index++;
   }
   while( return_value != END_OF_BASE64_ENCODED_DATA &&
          decoder_table[ return_value ] == BASE64_IGNORABLE_CHARACTER );

   return( return_value );
}

CBase64Coding::CBase64Coding()
{
}

CBase64Coding::~CBase64Coding()
{
}

BOOL CBase64Coding::Encode( const char * source, int len, char * destination_string )
{

   const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

   int loop_index                = 0;
   int number_of_bytes_to_encode = len;

   BYTE byte_to_add = 0;
   BYTE byte_1      = 0;
   BYTE byte_2      = 0;
   BYTE byte_3      = 0;

   DWORD number_of_bytes_encoded = (DWORD) ( (double) number_of_bytes_to_encode / (double) 0.75 ) + 1;

   // Now add in the CR/LF pairs, each line is truncated at 72 characters

   // 2000-05-12
   // Thanks go to Ilia Golubev (ilia@varicom.co.il) for finding a bug here.
   // I was using number_of_bytes_to_encode rather than number_of_bytes_encoded.

   number_of_bytes_encoded += (DWORD)( ( ( number_of_bytes_encoded / BASE64_NUMBER_OF_CHARACTERS_PER_LINE ) + 1 ) * 2 );

   char * destination = destination_string;

   number_of_bytes_encoded = 0;

   while( loop_index < number_of_bytes_to_encode )
   {
      // Output the first byte

      byte_1 = source[ loop_index ];
      byte_to_add = alphabet[ ( byte_1 >> 2 ) ];

      destination[ number_of_bytes_encoded ] = static_cast< char >( byte_to_add );
      number_of_bytes_encoded++;

      loop_index++;

      if ( loop_index >= number_of_bytes_to_encode )
      {
         // We're at the end of the data to encode

         byte_2 = 0;
         byte_to_add = alphabet[ ( ( ( byte_1 & 0x03 ) << 4 ) | ( ( byte_2 & 0xF0 ) >> 4 ) ) ];

         destination[ number_of_bytes_encoded ] = byte_to_add;
         number_of_bytes_encoded++;

         destination[ number_of_bytes_encoded ] =  END_OF_BASE64_ENCODED_DATA;
         number_of_bytes_encoded++;

         destination[ number_of_bytes_encoded ] =  END_OF_BASE64_ENCODED_DATA;

         // 1999-09-01
         // Thanks go to Yurong Lin (ylin@dial.pipex.com) for finding a bug here.
         // We must NULL terminate the string before letting CString have the buffer back.

         destination[ number_of_bytes_encoded + 1 ] = 0;

         return( TRUE );
      }
      else
      {
         byte_2 = source[ loop_index ];
      }

      byte_to_add = alphabet[ ( ( ( byte_1 & 0x03 ) << 4 ) | ( ( byte_2 & 0xF0 ) >> 4 ) ) ];

      destination[ number_of_bytes_encoded ] = byte_to_add;
      number_of_bytes_encoded++;

      loop_index++;

      if ( loop_index >= number_of_bytes_to_encode )
      {
         // We ran out of bytes, we need to add the last half of byte_2 and pad
         byte_3 = 0;

         byte_to_add = alphabet[ ( ( ( byte_2 & 0x0F ) << 2 ) | ( ( byte_3 & 0xC0 ) >> 6 ) ) ];

         destination[ number_of_bytes_encoded ] = byte_to_add;
         number_of_bytes_encoded++;

         destination[ number_of_bytes_encoded ] = END_OF_BASE64_ENCODED_DATA;
         destination[ number_of_bytes_encoded + 1 ] = 0;
         return( TRUE );
      }
      else
      {
         byte_3 = source[ loop_index ];
      }

      loop_index++;

      byte_to_add = alphabet[ ( ( ( byte_2 & 0x0F ) << 2 ) | ( ( byte_3 & 0xC0 ) >> 6 ) ) ];

      destination[ number_of_bytes_encoded ] = byte_to_add;
      number_of_bytes_encoded++;

      byte_to_add = alphabet[ ( byte_3 & 0x3F ) ];

      destination[ number_of_bytes_encoded ] = byte_to_add;
      number_of_bytes_encoded++;

      if ( ( number_of_bytes_encoded % BASE64_NUMBER_OF_CHARACTERS_PER_LINE ) == 0 )
      {
         destination[ number_of_bytes_encoded ] = CARRIAGE_RETURN;
         number_of_bytes_encoded++;

         destination[ number_of_bytes_encoded ] = LINE_FEED;
         number_of_bytes_encoded++;
      }
   }

   destination[ number_of_bytes_encoded ] = END_OF_BASE64_ENCODED_DATA;
   destination[ number_of_bytes_encoded + 1 ] = 0;
   return( TRUE );
}
