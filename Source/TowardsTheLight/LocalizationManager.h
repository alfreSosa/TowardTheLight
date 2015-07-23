// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class TOWARDSTHELIGHT_API LocalizationManager {
public:
  static LocalizationManager* Instance();

protected:
	LocalizationManager();
	~LocalizationManager();

private:
  static LocalizationManager* m_instance;
};
