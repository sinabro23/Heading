// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"
#include "Enemy.h"

UMonsterAnimInstance::UMonsterAnimInstance()
{

}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	auto Pawn = TryGetPawnOwner();
	if (Pawn)
	{
		AEnemy* Enemy = Cast<AEnemy>(Pawn);
		if (Enemy)
		{
			Speed = Enemy->GetVelocity().Size();
		}
	}
}
