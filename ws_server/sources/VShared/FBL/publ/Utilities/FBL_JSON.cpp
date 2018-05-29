/**********************************************************************************************/
/* FBL_JSON.cpp																				  */
/*                                                                       					  */
/* Copyright Paradigma, 1998-2017															  */
/* All Rights Reserved.                                                 					  */
/**********************************************************************************************/

#include <VShared/FBL/publ/Headers/StdAfx.h>
#include <VShared/FBL/publ/Util_classes/FBL_String.h>
#include <VShared/FBL/publ/Util_classes/FBL_StringConversion.h>
#include <VShared/FBL/publ/Values/FBL_Value_String.h>
#include <VShared/FBL/publ/Values/FBL_Value_Numeric.h>
#include <VShared/FBL/publ/Values/FBL_Value_Double.h>

#include <VShared/FBL/publ/Utilities/FBL_JSON.h>
#include <VShared/FBL/publ/Utilities/cjson/cJSON.h>


#include <stdlib.h>

/**********************************************************************************************/
FBL_Begin_Namespace

/**********************************************************************************************/
static unsigned char ctype[256];


/**********************************************************************************************/
static const struct ctype_init_t
{
	ctype_init_t()
	{
		ctype[(int)'\0'] |= 002;
		for( const char* s = "\t\n\v\f\r\x20"; *s; ++s )
			ctype[(int)*s] |= 001;
		for( const char* s = ",:]}"; *s; ++s )
			ctype[(int)*s] |= 002;
		for( const char* s = "+-"; *s; ++s )
			ctype[(int)*s] |= 004;
		for( const char* s = "0123456789"; *s; ++s )
			ctype[(int)*s] |= 010;
		for( const char* s = "ABCDEF" "abcdef"; *s; ++s )
			ctype[(int)*s] |= 020;
	}
} ctype_init;


/**********************************************************************************************/
inline bool is_space( char c )
{
	return (ctype[(int)(unsigned char)c] & 001) != 0;
}


/**********************************************************************************************/
inline bool is_delim( char c )
{
	return (ctype[(int)(unsigned char)c] & 003) != 0;
}


/**********************************************************************************************/
inline bool is_sign( char c )
{
	return (ctype[(int)(unsigned char)c] & 004) != 0;
}


/**********************************************************************************************/
inline bool is_dec( char c )
{
	return (ctype[(int)(unsigned char)c] & 010) != 0;
}

/**********************************************************************************************/
inline bool is_hex( char c )
{
	return (ctype[(int)(unsigned char)c] & 030) != 0;
}


/**********************************************************************************************/
inline int char2int( char c )
{
	if( c >= 'a' )
		return c - 'a' + 10;
	if( c >= 'A' )
		return c - 'A' + 10;
	return c - '0';
}


/**********************************************************************************************/
static double str2float( const char* str, char** endptr )
{
	double sign = is_sign(*str) && *str++ == '-' ? -1 : 1;
	double result = 0;
	
	while( is_dec(*str) )
		result = (result * 10) + (*str++ - '0');
		
	if( *str == '.' )
	{
		++str;
		double fraction = 1;
		while( is_dec(*str) )
			fraction *= 0.1, result += (*str++ - '0') * fraction;
	}
	
	if( *str == 'e' || *str == 'E' )
	{
		++str;
		double base = is_sign(*str) && *str++ == '-' ? 0.1 : 10;
		int exponent = 0;
		while( is_dec(*str) )
			exponent = (exponent * 10) + (*str++ - '0');
		double power = 1;
		for( ; exponent; exponent >>= 1, base *= base )
			if( exponent & 1 )
				power *= base;
				
		result *= power;
	}
	
	*endptr = (char*)str;
	return sign * result;
}


/**********************************************************************************************/
JsonAllocator::~JsonAllocator()
{
	while (head)
	{
		Zone* temp = head->next;
		free(head);
		head = temp;
	}
}


/**********************************************************************************************/
inline void* align_pointer( void* x, size_t align )
{
	return (void*)( ((uintptr_t)x + (align - 1)) & ~(align - 1) );
}


/**********************************************************************************************/
void* JsonAllocator::allocate( size_t n, size_t align )
{
	if( head )
	{
		char* p = (char*)align_pointer( head->end, align );
		if( p + n <= (char*)head + JSON_ZONE_SIZE )
		{
			head->end = p + n;
			return p;
		}
	}
	
	size_t zone_size = sizeof(Zone) + n + align;
	Zone* z = (Zone*) malloc( zone_size <= JSON_ZONE_SIZE ? JSON_ZONE_SIZE : zone_size );

	char* p = nullptr;
	if( z )
	{
		p = (char*) align_pointer( z + 1, align );
		z->end = p + n;
		if( zone_size <= JSON_ZONE_SIZE || head == nullptr )
		{
			z->next = head;
			head = z;
		}
		else
		{
			z->next = head->next;
			head->next = z;
		}
	}		

	return p;
}


/**********************************************************************************************/
struct JsonList
{
#if FBL_WIN
		JsonList( void )
		:
			tag(JSON_TAG_NULL),
			node(0),
			key(nullptr)
		{
		}

		JsonList( JsonTag inTag, JsonValue inValue, char* inKey )
		:
			tag(inTag),
			node(inValue),
			key(inKey)
		{
		}
#endif // FBL_WIN

	JsonTag			tag;
	JsonValue		node;
	char*			key;

	void grow_the_tail( JsonNode* p )
	{
		JsonNode* tail = (JsonNode*) node.getPayload();
		if( tail )
		{
			p->next = tail->next;
			tail->next = p;
		}
		else
		{
			p->next = p;
		}
		node = JsonValue( tag, p );
	}

	JsonValue cut_the_head()
	{
		JsonNode* tail = (JsonNode*)node.getPayload();
		if( tail )
		{
			JsonNode* head = tail->next;
			tail->next = nullptr;
			return JsonValue( tag, head );
		}
		
		return node;
	}
};


#pragma mark -

/**********************************************************************************************/
JsonParseStatus json_parse( char* str, char** endptr, JsonValue* value, JsonAllocator& allocator )
{
	JsonList stack[JSON_STACK_SIZE];
	int top = -1;
	bool separator = true;
	while( *str )
	{
		JsonValue o;
		while( *str && is_space(*str) )
			++str;
			
		*endptr = str++;
		switch( **endptr )
		{
			case '\0':
				continue;
			case '-':
				if( !is_dec(*str) && *str != '.')
					return *endptr = str, JSON_PARSE_BAD_NUMBER;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				o = JsonValue( str2float(*endptr, &str) );
				if( !is_delim(*str) )
					return *endptr = str, JSON_PARSE_BAD_NUMBER;
			break;
			case '"':
				o = JsonValue( JSON_TAG_STRING, str );
				for( char *s = str; *str; ++s, ++str )
				{
					int c = *s = *str;
					if( c == '\\' )
					{
						c = *++str;
						switch( c )
						{
							case '\\':
							case '"':
							case '/': *s = (char) c; break;
							case 'b': *s = '\b'; break;
							case 'f': *s = '\f'; break;
							case 'n': *s = '\n'; break;
							case 'r': *s = '\r'; break;
							case 't': *s = '\t'; break;
							case 'u':
								c = 0;
								for( int i = 0; i < 4; ++i )
								{
									if( !is_hex(*++str) )
										return *endptr = str, JSON_PARSE_BAD_STRING;
									c = c * 16 + char2int(*str);
								}
								if( c < 0x80 )
								{
									*s = (char) c;
								}
								else if( c < 0x800 )
								{
									*s++ = (char)( 0xC0 | (c >> 6) );
									*s = 0x80 | (c & 0x3F);
								}
								else
								{
									*s++ = (char)( 0xE0 | (c >> 12) );
									*s++ = 0x80 | ((c >> 6) & 0x3F);
									*s = 0x80 | (c & 0x3F);
								}
							break;
							default:
								return *endptr = str, JSON_PARSE_BAD_STRING;
						}
					}
					else if( c == '"' )
					{
						*s = 0;
						++str;
						break;
					}
				}
				if( !is_delim(*str) )
					return *endptr = str, JSON_PARSE_BAD_STRING;
			break;
			
			case 't':
				for( const char* s = "rue"; *s; ++s, ++str )
				{
					if( *s != *str )
						return JSON_PARSE_BAD_IDENTIFIER;
				}
				if( !is_delim(*str) )
					return JSON_PARSE_BAD_IDENTIFIER;
					
				o = JsonValue( JSON_TAG_BOOL, (void*)true );
			break;
			
			case 'f':
				for( const char* s = "alse"; *s; ++s, ++str )
				{
					if( *s != *str )
						return JSON_PARSE_BAD_IDENTIFIER;
				}
				
				if( !is_delim(*str) )
					return JSON_PARSE_BAD_IDENTIFIER;
					
				o = JsonValue( JSON_TAG_BOOL, (void*)false );
			break;
			
			case 'n':
				for( const char* s = "ull"; *s; ++s, ++str )
				{
					if( *s != *str )
						return JSON_PARSE_BAD_IDENTIFIER;
				}
				
				if( !is_delim(*str) )
					return JSON_PARSE_BAD_IDENTIFIER;
			break;
			
			case ']':
				if( top == -1 )
					return JSON_PARSE_STACK_UNDERFLOW;
					
				if( stack[top].tag != JSON_TAG_ARRAY )
					return JSON_PARSE_MISMATCH_BRACKET;
					
				o = stack[top--].cut_the_head();
			break;
			
			case '}':
				if( top == -1 )
					return JSON_PARSE_STACK_UNDERFLOW;
					
				if( stack[top].tag != JSON_TAG_OBJECT )
					return JSON_PARSE_MISMATCH_BRACKET;
					
				o = stack[top--].cut_the_head();
			break;
			
			case '[':
				if( ++top == JSON_STACK_SIZE )
					return JSON_PARSE_STACK_OVERFLOW;

#if FBL_WIN
				stack[top] = JsonList( JSON_TAG_ARRAY, JsonValue(JSON_TAG_ARRAY, nullptr), nullptr );
#else
				stack[top] = { JSON_TAG_ARRAY, JsonValue(JSON_TAG_ARRAY, nullptr), nullptr };
#endif // FBL_WIN
				continue;
			
			case '{':
				if( ++top == JSON_STACK_SIZE )
					return JSON_PARSE_STACK_OVERFLOW;

#if FBL_WIN
				stack[top] = JsonList( JSON_TAG_OBJECT, JsonValue(JSON_TAG_OBJECT, nullptr), nullptr );
#else
				stack[top] = { JSON_TAG_OBJECT, JsonValue(JSON_TAG_OBJECT, nullptr), nullptr };
#endif // FBL_WIN
				continue;
				
			case ':':
				if( separator || stack[top].key == nullptr )
					return JSON_PARSE_UNEXPECTED_CHARACTER;
					
				separator = true;
				continue;
				
			case ',':
				if( separator || stack[top].key != nullptr )
					return JSON_PARSE_UNEXPECTED_CHARACTER;
					
				separator = true;
				continue;
				
			default:
				return JSON_PARSE_UNEXPECTED_CHARACTER;
		}

		separator = false;

		if( top == -1 )
		{
			*endptr = str;
			*value = o;
			return JSON_PARSE_OK;
		}

		if( stack[top].tag == JSON_TAG_OBJECT )
		{
			if( !stack[top].key )
			{
				if( o.getTag() != JSON_TAG_STRING )
					return JSON_PARSE_UNQUOTED_KEY;
					
				stack[top].key = o.toString();
				continue;
			}
			
			JsonNode* p = (JsonNode*)allocator.allocate( sizeof(JsonNode) );
			p->value = o;
			p->key = stack[top].key;
			stack[top].key = nullptr;
			stack[top].grow_the_tail( (JsonNode*)p );
			continue;
		}

		JsonNode* p = (JsonNode*)allocator.allocate( sizeof(JsonNode) - sizeof(char*) );
		p->value = o;
		stack[top].grow_the_tail(p);
	}
	
	return JSON_PARSE_BREAKING_BAD;
}


/**********************************************************************************************/
bool GetSimpleValue( JsonValue inSource, const String& inKey, String& outValue )
{
	bool result = false;

#if FBL_WIN
	for( JsonIterator i = begin(inSource); i != end(inSource); ++i )
#else
	for( auto i : inSource )
#endif // FBL_WIN	
	{
		if(	inKey.caseCompare( i->key, 0 ) == 0 )
		{
			result = true;
		
			switch( i->value.getTag() )
			{
				case JSON_TAG_NUMBER:
					outValue << i->value.toNumber();
				break;
				
				case JSON_TAG_BOOL:
					outValue << (i->value.toBool() ? "true" : "false");
				break;
				
				case JSON_TAG_STRING:
					outValue << i->value.toString();
				break;
				
				default:
					result = false;
			}
		}
	}
	
	return result;
}


#pragma mark -

/**********************************************************************************************/
Json::Json( const char* inJsonStr )
{
	mpRoot = cJSON_Parse( inJsonStr );
}


/**********************************************************************************************/
Json::~Json()
{
	if( mpRoot )
		cJSON_Delete( mpRoot );
}


/**********************************************************************************************/
bool Json::isValid( void )
{
	return  (bool)mpRoot;
}


/**********************************************************************************************/
String Json::get_ErrorStr( void )
{
	return cJSON_GetErrorPtr();
}


/**********************************************************************************************/
String Json::get_Value( const char* inpKey )
{
	String res ;
	
	if( mpRoot )
	{
		cJSON* item = cJSON_GetObjectItem( mpRoot, inpKey );
		if( item && item->valuestring )
		{
			FBL_ENGINE_LOCK
			StToUTF16 utf16Value( item->valuestring, -1, GetConverter_UTF8() );
			res = utf16Value.c_str();
		}
	}
	
	return res;
}


/**********************************************************************************************/
ArrayOfValueArrays_Ptr Json::get_Matrix( const char* inpKey )
{
	// We are expecting either array of "plain" values or array of arrays (which consist of
	// "plain" values). So we produce single-row of values in first case and multi-rows
	// in second case.
	//
	ArrayOfValueArrays_Ptr res;

	if( mpRoot )
	{
		cJSON* root = cJSON_GetObjectItem( mpRoot, inpKey );
		if( root && root->type == cJSON_Array )
		{
			res = new ArrayOfValueArrays();
			if( root->child )
			{
				if( root->child->type == cJSON_Array )
				{
					// Array of arrays ( [[1, "Ivanov" ...], [2, "Petrov" ...] ...] )
					// root->child is an array of arrays:
					//	0. pFirstArray = root->child
					//		0. pFirstArray->child
					//		1. pFirstArray->child->next
					//		2. pFirstArray->child->next->next
					//		....
					//	1. pSecondArray = pFirstArray->next
					//	....
					//
					cJSON* pArray = root->child;
					do
					{
						res->AddItem( get_Array( pArray ) );
						pArray = pArray->next;
					} while( pArray );
				}
				else
				{
					// Array of "plain" values ( [1, "Ivanov", null ...] )
					res->AddItem( get_Array( root ) );
				}
			}
		}
		else
		{
			// Do nothing.
			// inpKey is not found or not an array.
			// Return null in both cases. Let vKernel throws errors if params were expected.
		}
	}
	
	return res;
}


/**********************************************************************************************/
ArrayOfValues_Ptr Json::get_Array( cJSON* inpArray )
{
	FBL_CHECK( inpArray );
	ArrayOfValues_Ptr res;

	if( inpArray->type == cJSON_Array )
	{
		cJSON* pPlainItem = inpArray->child;
		if( pPlainItem )
		{
			res = new ArrayOfValues;
			do
			{
				switch( pPlainItem->type )
				{
					case cJSON_False:
					{
						I_Value_Ptr pValue = new Value_bool();
						pValue->put_Boolean(false);
						res->AddItem( pValue );
					} break;

					case cJSON_True:
					{
						I_Value_Ptr pValue = new Value_bool();
						pValue->put_Boolean(true);
						res->AddItem( pValue );
					} break;
					
					case cJSON_NULL:
					{
						I_Value_Ptr pValue = new Value_bool_null();
						pValue->put_IsNull(true);
						res->AddItem( pValue );
					} break;
					
					case cJSON_Number:
					{
						I_Value_Ptr pValue = new Value_double();
						pValue->put_Double( pPlainItem->valuedouble );
						res->AddItem( pValue );
					} break;
					
					case cJSON_String:
					{
						FBL_ENGINE_LOCK
						StToUTF16 utf16Value( pPlainItem->valuestring, -1, GetConverter_UTF8() );
						I_Value_Ptr pValue = new Value_string( utf16Value.length() );
						pValue->put_String( utf16Value.c_str() );
						res->AddItem( pValue );
					} break;
					
					default:
					{
						// Do nothing.
						// Unexpected data type.
						// Add null as a value. Let vKernel throws errors trying to work with
						// nullptr as bind value.
						//
						res->AddItem( nullptr );
					} break;
				}
				
				pPlainItem = pPlainItem->next;
			} while( pPlainItem );
		}
		else
		{
			// Do nothing.
			// inpArray is not an array.
			// Return null. Let vKernel throws errors if params were expected.
		}
	}
	
	return res;
}


/**********************************************************************************************/
FBL_End_Namespace
