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

	FName GetAttackMontageName(int32 MontageSection); // ���� ���� �־��ָ� �� ���ڿ� �´� ���� �̸�
private:
	UFUNCTION()
	void AnimNotify_AttackHit(); // �Լ� �̸� �԰����� 

public:
	FOnAttackHit OnAttackHit;
};