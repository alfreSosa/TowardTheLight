// Fill out your copyright notice in the Description page of Project Settings.

#include "TowardsTheLight.h"
#include "TowardsTheLightGameMode.h"
#include "PlayerOvi.h"
#include "OviPlayerController.h"
#include "GameDataManager.h"
#include "LocalizationManager.h"
#include "TimeManager.h"
#include "PickableItem.h"
#include "SoundManager.h"

ATowardsTheLightGameMode::ATowardsTheLightGameMode(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
  PrimaryActorTick.bCanEverTick = true;

//  DefaultPawnClass = APlayerOvi::StaticClass();
  PlayerControllerClass = AOviPlayerController::StaticClass();

  static ConstructorHelpers::FObjectFinder<UBlueprint> SomeBlueprint(TEXT("Blueprint'/Game/UIElementsBP/LevelHUD.LevelHUD'"));
  if (SomeBlueprint.Object)
    HUDClass = (UClass*)SomeBlueprint.Object->GeneratedClass;

  m_countOrbs = m_actualPoints = 0;
  state = EndGameType::NONE;

  //initialize checkpoint
  m_actualCheckPoint.IsPicked = false;
  m_player = nullptr;
  m_actualCheckPoint.PlayerHasKey = false;
  m_actualCheckPoint.ColorKey = FLinearColor(1, 1, 1);
}

void ATowardsTheLightGameMode::EndGame(EndGameType type) {
  switch (type){
  case VICTORY:{
    if (state == EndGameType::NONE){
      LevelData data = GameDataManager::Instance()->ReadLevelData(GetWorld()->GetMapName());
      //si la puntuacion actual es mejor que la que hay en el fichero, hay que almacenarla 
      bool write = false;
      if (m_countOrbs >= data.orbs){
        data.orbs = m_countOrbs;
        write = true;
      }
      if (m_actualPoints >= data.points){
        data.points = m_actualPoints;
        write = true;
      }
      if (write)
        GameDataManager::Instance()->WriteLevelData(data);
    }

    //terminar la partida. volver al menú
    state = EndGameType::VICTORY;
  }
               break;
  case DEFEAT:
    //terminar la partida. volver al menú
    state = EndGameType::DEFEAT;
    break;
  }
}

void ATowardsTheLightGameMode::AddPoints(float points) {
  m_actualPoints += points;
  //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ActualPoints: %f"), m_actualPoints));
  PointsSoundEvent();
}
void ATowardsTheLightGameMode::OrbPicked() {
  m_countOrbs++;
  //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Orbs: %d"), m_countOrbs));
  OrbsSoundEvent();
}

float ATowardsTheLightGameMode::GetActualPoints() {
  return m_actualPoints;
}
float ATowardsTheLightGameMode::GetActualOrbs() {
  return m_countOrbs;
}

void ATowardsTheLightGameMode::SetPauseBP(bool enable) {
  TimeManager::Instance()->SetPause(enable);
}
bool ATowardsTheLightGameMode::IsPausedBP() {
  return TimeManager::Instance()->IsPaused();
}
float ATowardsTheLightGameMode::EndGameBP() {
  return (float)state;
}

FString ATowardsTheLightGameMode::GetLevelNameBP(){
  FString levelName = GetWorld()->GetMapName();
  levelName.RemoveFromStart(FString("UEDPIE_0_"));
  return levelName;
}

void ATowardsTheLightGameMode::SetPlayerCheckPoint(APlayerOvi *player, FTransform playerStatus, bool right) {
  //storage here points and orbs to avoid other functions
  m_actualCheckPoint.Points = m_actualPoints;
  m_actualCheckPoint.Orbs = m_countOrbs;
  m_actualCheckPoint.IsPicked = true;
  m_actualCheckPoint.PlayerToRight = right;
  m_actualCheckPoint.PlayerStatus = playerStatus;
  m_player = player;
}

void ATowardsTheLightGameMode::SetPlayerKey(bool hasKey, FLinearColor colorKey) {
  m_actualCheckPoint.PlayerHasKey = hasKey;
  m_actualCheckPoint.ColorKey = colorKey;
}

void ATowardsTheLightGameMode::AddItemPicked(APickableItem *item) {
  //si no estan ya, lo guardo
  if (m_actualCheckPoint.ItemsPicked.Find(item) == INDEX_NONE)
    m_actualCheckPoint.ItemsPicked.Add(item);
}

bool ATowardsTheLightGameMode::IsCheckPointPicked() {
  return m_actualCheckPoint.IsPicked;
}

void ATowardsTheLightGameMode::RestoreLevel(bool checkPoint) {
  if (checkPoint) {
    m_actualPoints = m_actualCheckPoint.Points;
    m_countOrbs = m_actualCheckPoint.Orbs;
    m_player->ResetToCheckPoint(m_actualCheckPoint.PlayerStatus, m_actualCheckPoint.PlayerToRight);
    m_player->SetKey(m_actualCheckPoint.PlayerHasKey, m_actualCheckPoint.ColorKey);
    state = EndGameType::NONE;
    //prueba de restaurar item cogidos despues del checkpoint
    /*Si los he cogido y no estan en el array almacenado los restauro*/
    for (TActorIterator<APickableItem> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
      if (ActorItr->IsItemPicked()) {
        if (m_actualCheckPoint.ItemsPicked.Find(*ActorItr) == INDEX_NONE) {
          (*ActorItr)->RestoreInitialPosition();
        }
      }
    }
  }
}

float ATowardsTheLightGameMode::GetLevelOrbs(FString levelName){
  return GameDataManager::Instance()->GetOrbsLevel(levelName);
}
float ATowardsTheLightGameMode::GetLevelPoints(FString levelName){
  return GameDataManager::Instance()->GetPointsLevel(levelName);
}

float ATowardsTheLightGameMode::GetTotalOrbs(){
  return GameDataManager::Instance()->GetOrbsCounts();
}

bool ATowardsTheLightGameMode::LevelExists(FString levelName){
  return GameDataManager::Instance()->LevelExists(levelName);
}

bool ATowardsTheLightGameMode::HasMusicBP(){
  return GameDataManager::Instance()->HasMusic();
}
void ATowardsTheLightGameMode::SetMusicBP(bool enable){
  GameDataManager::Instance()->SetMusic(enable);
  SoundManager::Instance()->SetMusic(enable);
}
bool ATowardsTheLightGameMode::HasEffectsBP(){
  return GameDataManager::Instance()->HasEffects();
}
void ATowardsTheLightGameMode::SetEffectsBP(bool enable){
  GameDataManager::Instance()->SetEffects(enable);
  SoundManager::Instance()->SetEffects(enable);
}

FString ATowardsTheLightGameMode::GetString(FString key){
  return LocalizationManager::Instance()->GetString(key);
}
void ATowardsTheLightGameMode::ChangeLanguage(FString language){
  LocalizationManager::Instance()->SetLanguage(language);
  GameDataManager::Instance()->SetLanguage(language);
}
