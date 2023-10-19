// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Weapon/WeaponAssetFactory.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWeaponAssetFactory() {}
// Cross Module References
	WEAPON_API UClass* Z_Construct_UClass_UWeaponAssetFactory_NoRegister();
	WEAPON_API UClass* Z_Construct_UClass_UWeaponAssetFactory();
	UNREALED_API UClass* Z_Construct_UClass_UFactory();
	UPackage* Z_Construct_UPackage__Script_Weapon();
// End Cross Module References
	void UWeaponAssetFactory::StaticRegisterNativesUWeaponAssetFactory()
	{
	}
	UClass* Z_Construct_UClass_UWeaponAssetFactory_NoRegister()
	{
		return UWeaponAssetFactory::StaticClass();
	}
	struct Z_Construct_UClass_UWeaponAssetFactory_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UWeaponAssetFactory_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UFactory,
		(UObject* (*)())Z_Construct_UPackage__Script_Weapon,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWeaponAssetFactory_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "WeaponAssetFactory.h" },
		{ "ModuleRelativePath", "WeaponAssetFactory.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UWeaponAssetFactory_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWeaponAssetFactory>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UWeaponAssetFactory_Statics::ClassParams = {
		&UWeaponAssetFactory::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UWeaponAssetFactory_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UWeaponAssetFactory_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UWeaponAssetFactory()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UWeaponAssetFactory_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UWeaponAssetFactory, 624121323);
	template<> WEAPON_API UClass* StaticClass<UWeaponAssetFactory>()
	{
		return UWeaponAssetFactory::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UWeaponAssetFactory(Z_Construct_UClass_UWeaponAssetFactory, &UWeaponAssetFactory::StaticClass, TEXT("/Script/Weapon"), TEXT("UWeaponAssetFactory"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UWeaponAssetFactory);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
