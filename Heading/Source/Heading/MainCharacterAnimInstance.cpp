// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAnimInstance.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UMainCharacterAnimInstance::UMainCharacterAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("AnimMontage'/Game/_MyContents/Character/Animation/AttackMontage.AttackMontage'"));
	if (AM.Succeeded())
	{
		AttackMontage = AM.Object;
	}
}

void UMainCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (Pawn)
	{
		FVector SpeedSize = Pawn->GetVelocity();
		SpeedSize.Z = 0.f;
		Speed = SpeedSize.Size();

		auto Character = Cast<AMainCharacter>(Pawn);
		if (Character)
		{
			IsFalling = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UMainCharacterAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.f);
}

void UMainCharacterAnimInstance::JumpToSection(int32 MontageSection)
{
	FName Name = GetAttackMontageName(MontageSection);
	Montage_JumpToSection(Name, AttackMontage);
}

FName UMainCharacterAnimInstance::GetAttackMontageName(int32 MontageSection)
{
	return FName(*FString::Printf(TEXT("Attack%d"), MontageSection)); // ���ø�Ÿ�ֿ��� ���� ���� �������°�
}

void UMainCharacterAnimInstance::AnimNotify_AttackHit()
{
	//UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackHit"));
	OnAttackHit.Broadcast(); // �� �Լ����� ����ϴ°ſ� �����ִ� ��ü���� OnAttackHit�� �����ؼ� �޽����� �޾ƺ��� �ȴ�. �̰�쿡�� ����ĳ���Ϳ��� ��������
}
