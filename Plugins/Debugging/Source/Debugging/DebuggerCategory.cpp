#include "DebuggerCategory.h"
#include "CanvasItem.h"
#include "DrawDebugHelpers.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
//#include "Utilities/CHelpers.h"

FDebuggerCategory::FDebuggerCategory()
{
	bShowOnlyWithDebugActor = false;
}

FDebuggerCategory::~FDebuggerCategory()
{

}

TSharedRef<FGameplayDebuggerCategory> FDebuggerCategory::MakeInstance()
{
	return MakeShareable(new FDebuggerCategory());
}

void FDebuggerCategory::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	FGameplayDebuggerCategory::CollectData(OwnerPC, DebugActor);

	//GLog->Log(OwnerPC->GetPawn()->GetName());

	//if (!!DebugActor)
	//	GLog->Log(DebugActor->GetName());

	ACharacter* player = OwnerPC->GetPawn<ACharacter>();
	if (player == nullptr) return;

	//Player
	{
		PlayerPawnData.bDraw = true;
		PlayerPawnData.Name = player->GetName();
		PlayerPawnData.Location = player->GetActorLocation();
		PlayerPawnData.Forward = player->GetActorForwardVector();

		UActorComponent* state = player->GetComponentByClass(UCStateComponent::StaticClass());
		PlayerPawnData.State = Cast<UCStateComponent>(state);

		UActorComponent* weapon = player->GetComponentByClass(UCWeaponComponent::StaticClass());
		PlayerPawnData.Weapon = Cast<UCWeaponComponent>(weapon);

		if (!!PlayerPawnData.State)
		{
			PlayerPawnData.StateString = PlayerPawnData.State->GetStateTypeAsString();
		}

		if (!!PlayerPawnData.Weapon)
		{
			PlayerPawnData.WeaponString = PlayerPawnData.Weapon->GetWeaponTypeAsString();
		}
		
		
	}

	//DebugActor
	{
		if (!!DebugActor)
		{
			DebugActorData.bDraw = true;
			DebugActorData.Name = DebugActor->GetName();
			DebugActorData.Location = DebugActor->GetActorLocation();
			DebugActorData.Forward = DebugActor->GetActorForwardVector();

			UActorComponent* state = DebugActor->GetComponentByClass(UCStateComponent::StaticClass());
			DebugActorData.State = Cast<UCStateComponent>(state);

			UActorComponent* weapon = DebugActor->GetComponentByClass(UCWeaponComponent::StaticClass());
			DebugActorData.Weapon = Cast<UCWeaponComponent>(weapon);

			if (!!DebugActorData.State)
			{
				DebugActorData.StateString = DebugActorData.State->GetStateTypeAsString();
			}

			if (DebugActorData.Weapon)
			{
				DebugActorData.WeaponString = DebugActorData.Weapon->GetWeaponTypeAsString();
			}
		} 
	}

	//Forward Actor
	{
		FHitResult hitResult;

		FVector start = PlayerPawnData.Location;
		FVector end = start + player->GetActorForwardVector() * TraceDistance;


		FCollisionQueryParams params;
		params.AddIgnoredActor(player);

		player->GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, params);

		if (hitResult.bBlockingHit)
		{
			ForwardActorData.bDraw = true;
			ForwardActorData.Name = hitResult.GetActor()->GetName();
			ForwardActorData.Location = hitResult.GetActor()->GetActorLocation();
			ForwardActorData.Forward = hitResult.GetActor()->GetActorForwardVector();

			UActorComponent* state = hitResult.GetActor()->GetComponentByClass(UCStateComponent::StaticClass());
			ForwardActorData.State = Cast<UCStateComponent>(state);

			UActorComponent* weapon = hitResult.GetActor()->GetComponentByClass(UCWeaponComponent::StaticClass());
			ForwardActorData.Weapon = Cast<UCWeaponComponent>(weapon);

			if (!!ForwardActorData.State)
			{
				ForwardActorData.StateString = ForwardActorData.State->GetStateTypeAsString();
			}

			if (!!ForwardActorData.Weapon)
			{
				ForwardActorData.WeaponString = ForwardActorData.Weapon->GetWeaponTypeAsString();
			}
			
			
		}
	}
}

void FDebuggerCategory::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	FGameplayDebuggerCategory::DrawData(OwnerPC, CanvasContext);

	FVector start = PlayerPawnData.Location;
	FVector end = start + PlayerPawnData.Forward * TraceDistance;

	DrawDebugLine(OwnerPC->GetWorld(), start, end, FColor::Red);

	FCanvasTileItem item(FVector2D(10, 10), FVector2D(320, 450), FLinearColor(0, 0, 0, 0.25f));
	item.BlendMode = ESimpleElementBlendMode::SE_BLEND_AlphaBlend;
	CanvasContext.DrawItem(item, CanvasContext.CursorX, CanvasContext.CursorY);

	CanvasContext.Printf(FColor::Green, L"  -- Player Pawn --");
	CanvasContext.Printf(FColor::White, L"  Name : %s", *PlayerPawnData.Name);
	CanvasContext.Printf(FColor::White, L"  Location : %s", *PlayerPawnData.Location.ToString());
	CanvasContext.Printf(FColor::White, L"  Forward : %s", *PlayerPawnData.Forward.ToString());
	CanvasContext.Printf(FColor::Red, L"  Player State : %s", *PlayerPawnData.StateString);
	CanvasContext.Printf(FColor::Blue, L"  Player Weapon : %s", *PlayerPawnData.WeaponString);
	CanvasContext.Printf(FColor::White, L"");

	if (DebugActorData.bDraw)
	{
		CanvasContext.Printf(FColor::Green, L"  -- Select Actor --");
		CanvasContext.Printf(FColor::White, L"  Name : %s", *DebugActorData.Name);
		CanvasContext.Printf(FColor::White, L"  Location : %s", *DebugActorData.Location.ToString());
		CanvasContext.Printf(FColor::White, L"  Forward : %s", *DebugActorData.Forward.ToString());
		CanvasContext.Printf(FColor::Red, L"  DebugActor State : %s", *DebugActorData.StateString);
		CanvasContext.Printf(FColor::Blue, L"  DebugActor Weapon : %s", *DebugActorData.WeaponString);
		CanvasContext.Printf(FColor::White, L"");
	}

	if (ForwardActorData.bDraw)
	{
		CanvasContext.Printf(FColor::Green, L"  -- Forward Actor --");
		CanvasContext.Printf(FColor::White, L"  Name : %s", *ForwardActorData.Name);
		CanvasContext.Printf(FColor::White, L"  Location : %s", *ForwardActorData.Location.ToString());
		CanvasContext.Printf(FColor::White, L"  Forward : %s", *ForwardActorData.Forward.ToString());
		CanvasContext.Printf(FColor::Red, L"  Forward Actor State : %s", *ForwardActorData.StateString);
		CanvasContext.Printf(FColor::Blue, L"  Forward Actor Weapon : %s", *ForwardActorData.WeaponString);
		CanvasContext.Printf(FColor::White, L"");
	}


}
