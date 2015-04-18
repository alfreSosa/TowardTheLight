// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	C++ class header boilerplate exported from UnrealHeaderTool.
	This is automatically generated by the tools.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectBase.h"

#ifdef TOWARDSTHELIGHT_OviCameraActor_generated_h
#error "OviCameraActor.generated.h already included, missing '#pragma once' in OviCameraActor.h"
#endif
#define TOWARDSTHELIGHT_OviCameraActor_generated_h

#define AOviCameraActor_EVENTPARMS
#define AOviCameraActor_RPC_WRAPPERS
#define AOviCameraActor_RPC_WRAPPERS_NO_PURE_DECLS \
	static inline void StaticChecks_Implementation_Validate() \
	{ \
	}


#define AOviCameraActor_CALLBACK_WRAPPERS
#define AOviCameraActor_INCLASS_NO_PURE_DECLS \
	private: \
	static void StaticRegisterNativesAOviCameraActor(); \
	friend TOWARDSTHELIGHT_API class UClass* Z_Construct_UClass_AOviCameraActor(); \
	public: \
	DECLARE_CLASS(AOviCameraActor, ACameraActor, COMPILED_IN_FLAGS(0), 0, TowardsTheLight, NO_API) \
	DECLARE_SERIALIZER(AOviCameraActor) \
	/** Indicates whether the class is compiled into the engine */    enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	UObject* _getUObject() const { return const_cast<AOviCameraActor*>(this); }


#define AOviCameraActor_INCLASS \
	private: \
	static void StaticRegisterNativesAOviCameraActor(); \
	friend TOWARDSTHELIGHT_API class UClass* Z_Construct_UClass_AOviCameraActor(); \
	public: \
	DECLARE_CLASS(AOviCameraActor, ACameraActor, COMPILED_IN_FLAGS(0), 0, TowardsTheLight, NO_API) \
	DECLARE_SERIALIZER(AOviCameraActor) \
	/** Indicates whether the class is compiled into the engine */    enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	UObject* _getUObject() const { return const_cast<AOviCameraActor*>(this); }


#define AOviCameraActor_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AOviCameraActor(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AOviCameraActor) \
private: \
	/** Private copy-constructor, should never be used */ \
	NO_API AOviCameraActor(const AOviCameraActor& InCopy); \
public:


#define AOviCameraActor_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AOviCameraActor(const class FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private copy-constructor, should never be used */ \
	NO_API AOviCameraActor(const AOviCameraActor& InCopy); \
public: \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AOviCameraActor)


#undef UCLASS_CURRENT_FILE_NAME
#define UCLASS_CURRENT_FILE_NAME AOviCameraActor


#undef UCLASS
#undef UINTERFACE
#if UE_BUILD_DOCS
#define UCLASS(...)
#else
#define UCLASS(...) \
AOviCameraActor_EVENTPARMS
#endif


#undef GENERATED_UCLASS_BODY
#undef GENERATED_BODY
#undef GENERATED_IINTERFACE_BODY
#define GENERATED_UCLASS_BODY() \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	AOviCameraActor_RPC_WRAPPERS \
	AOviCameraActor_CALLBACK_WRAPPERS \
	AOviCameraActor_INCLASS \
	AOviCameraActor_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_POP


#define GENERATED_BODY() \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	AOviCameraActor_RPC_WRAPPERS_NO_PURE_DECLS \
	AOviCameraActor_CALLBACK_WRAPPERS \
	AOviCameraActor_INCLASS_NO_PURE_DECLS \
	AOviCameraActor_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_POP


