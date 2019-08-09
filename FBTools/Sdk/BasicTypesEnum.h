#pragma once

namespace fb
{
	enum BasicTypesEnum
	{
		BTE_Void,
		BTE_DbObject,
		BTE_ValueType,
		BTE_Class,
		BTE_Array,
		BTE_FixedArray,
		BTE_String,
		BTE_CString,
		BTE_Enum,
		BTE_FileRef,
		BTE_Boolean,
		BTE_Int8,
		BTE_Uint8,
		BTE_Int16,
		BTE_Uint16,
		BTE_Int32,
		BTE_Uint32,
		BTE_Int64,
		BTE_Uint64,
		BTE_Float32,
		BTE_Float64,
		BTE_Guid,
		BTE_SHA1,
		BTE_ResourceRef,
		BTECount
	};

	static const char* ToString( BasicTypesEnum value )
	{
		switch( value )
		{
			case BTE_Void:
				return "BTE_Void";
			case BTE_DbObject:
				return "BTE_DbObject";
			case BTE_ValueType:
				return "BTE_ValueType";
			case BTE_Class:
				return "BTE_Class";
			case BTE_Array:
				return "BTE_Array";
			case BTE_FixedArray:
				return "BTE_FixedArray";
			case BTE_String:
				return "BTE_String";
			case BTE_CString:
				return "BTE_CString";
			case BTE_Enum:
				return "BTE_Enum";
			case BTE_FileRef:
				return "BTE_FileRef";
			case BTE_Boolean:
				return "BTE_Boolean";
			case BTE_Int8:
				return "BTE_Int8";
			case BTE_Uint8:
				return "BTE_Uint8";
			case BTE_Int16:
				return "BTE_Int16";
			case BTE_Uint16:
				return "BTE_Uint16";
			case BTE_Int32:
				return "BTE_Int32";
			case BTE_Uint32:
				return "BTE_Uint32";
			case BTE_Int64:
				return "BTE_Int64";
			case BTE_Uint64:
				return "BTE_Uint64";
			case BTE_Float32:
				return "BTE_Float32";
			case BTE_Float64:
				return "BTE_Float64";
			case BTE_Guid:
				return "BTE_Guid";
			case BTE_SHA1:
				return "BTE_SHA1";
			case BTE_ResourceRef:
				return "BTE_ResourceRef";

			default:
				return "Unknown BasicTypesEnum value!";
		}
	}
}