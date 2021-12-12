#include "Ability/DexAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "Engine/Canvas.h"
#include "Character/DexCharacterBase.h"
#include "EngineUtils.h"

UDexAbilitySystemComponent::UDexAbilitySystemComponent(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{

}

void FDrawDebugTags::DrawDebugTags(UCanvas* Canvas, AActor* Actor, UAbilitySystemComponent* Comp, const FVector& CameraPos, const FVector& CameraDir, float& VerticalOffset)
{
	if (!Actor || !Comp || !Actor->WasRecentlyRendered(0.1f))
	{
		return;
	}
	
	FVector Loc = Actor->GetActorLocation();
	FVector CameraToActor = (Loc - CameraPos);
	// FVector operator '|' is dotproduct
	bool bIsBehindCamera = ((CameraDir | CameraToActor) < 0.f);
	if (bIsBehindCamera)
	{
		return;
	}
	
	bool bBeyondDistance = CameraToActor.SizeSquared2D() > FMath::Square(2048.f);
	if (bBeyondDistance)
	{
		return;
	}
	
	FGameplayTagContainer TagContainer;
	Comp->GetOwnedGameplayTags(TagContainer);
	if (!TagContainer.Num())
	{
		return;
	}
	
	FVector Origin;
	FVector BoxExtent;
	Actor->GetActorBounds(true, Origin, BoxExtent);
	
	FVector ScreenLoc = Canvas->Project(Origin);
	if ((ScreenLoc.X < 0.f) || (ScreenLoc.X >= Canvas->ClipX) || (ScreenLoc.Y < 0.f) || (ScreenLoc.Y > Canvas->ClipY))
	{
		return;
	}
	
	FGameplayTagContainer Leafs;
	for (auto ContainerIt = TagContainer.CreateConstIterator(); ContainerIt; ++ContainerIt)
	{
		Leafs.AddLeafTag(*ContainerIt);
	}

	UFont* Font = GEngine->GetMediumFont();
	TArray<FStringAndHeight> Strings;
	float MaxWidth = 0.f;
	float TotalHeight = 0.f;
	for (auto ContainerIt = Leafs.CreateConstIterator(); ContainerIt; ++ContainerIt)
	{
		FString DrawString = ContainerIt->ToString();
		float TextWidth = 0.f;
		float TextHeight = 0.f;
		Canvas->TextSize(Font, DrawString, TextWidth, TextHeight);
		MaxWidth = FMath::Max(MaxWidth, TextWidth);
		TotalHeight += TextHeight;

		// TArray<> will DestructItems() out of scope, clearing "new" allocations		
		new(Strings) FStringAndHeight(DrawString, TextHeight);
	}
	
	float TextLeft = ScreenLoc.X - MaxWidth * 0.5f;
	float TextTop = ScreenLoc.Y - TotalHeight * 0.5f + VerticalOffset;
	
	FLinearColor BackgroundColor(0.57758f, 0.184475f, 0.0065f, 0.3f);
	float BoxPadding = 2.f;
	FVector2D BoxLocation(TextLeft - BoxPadding, TextTop - BoxPadding);
	FVector2D BoxSize(MaxWidth + BoxPadding * 2.f, TotalHeight + BoxPadding * 2.f);
	VerticalOffset += BoxSize.Y + BoxPadding;
	FCanvasTileItem Tile(BoxLocation, BoxSize, BackgroundColor);
	Canvas->DrawItem(Tile);
	
	Canvas->SetDrawColor(FColor(255, 255, 255, 255));
	float DrawTop = TextTop;
	for (const FStringAndHeight& SL : Strings)
	{
		Canvas->DrawText(Font, SL.S, TextLeft, DrawTop);
		DrawTop += SL.H;
	}
}

void UDexAbilitySystemComponent::DrawAbilityTags(UCanvas* Canvas, APlayerController* PC)
{
	if (!PC)
	{
		return;
	}

	if (UWorld* World = PC->GetWorld())
	{
		FVector CamPosition;
		FRotator CamRotation;
		PC->GetPlayerViewPoint(CamPosition, CamRotation);
		FVector CamDirection = CamRotation.Vector();

		for (TActorIterator<ADexCharacterBase> CharacterIt(World); CharacterIt; ++CharacterIt)
		{
			ADexCharacterBase* Character = *CharacterIt;
			if (Character)
			{
				if (UAbilitySystemComponent* CharacterComp = Character->GetAbilitySystemComponent())
				{
					float VerticalOffset = 0.f;
					FDrawDebugTags::DrawDebugTags(Canvas, Character, CharacterComp, CamPosition, CamDirection, VerticalOffset);
				}
			}
		}
	}
}
