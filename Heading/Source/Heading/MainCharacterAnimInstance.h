// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainCharacterAnimInstance.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnAttackHit);
UCLASS()
class HEADING_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMainCharacterAnimInstance();

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

public:
	void PlayAttackMontage();
	void JumpToSection(int32 MontageSection);

	FName GetAttackMontageName(int32 MontageSection); // 섹션 숫자 넣어주면 그 숫자에 맞는 섹션 이름
private:
	UFUNCTION()
	void AnimNotify_AttackHit(); // 함수 이름 규격있음 

public:
	FOnAttackHit OnAttackHit;
};
