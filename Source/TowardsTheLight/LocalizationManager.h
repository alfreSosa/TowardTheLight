// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */

class LangDef {
public:
  LangDef() {}
  LangDef(const FString& name) { mName = name; }
  const FString& GetName() const { return mName; }
  void AddString(const FString& key, const FString& value) { mKeys.Add(key); mValues.Add(value); }
  const FString& GetString(const FString& key, const FString& defVal) const;

private:
  FString mName;
  TArray<FString> mKeys;
  TArray<FString> mValues;
};

class TOWARDSTHELIGHT_API LocalizationManager {
public:
  static LocalizationManager* Instance();

  bool ParseLanguage(const FString& filename);
  bool SetLanguage(const FString& name);
  const FString& GetCurrentLanguage() const { return mLanguages[mCurrentLanguage].GetName(); }
  const FString& GetString(const FString& key, const FString& defVal = "") const { return mLanguages[mCurrentLanguage].GetString(key, defVal); }

protected:
	LocalizationManager();
	~LocalizationManager();

private:
  static LocalizationManager* m_instance;
  TArray<LangDef> mLanguages;
  unsigned mCurrentLanguage;
};


