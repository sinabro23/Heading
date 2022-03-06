// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy.h"
// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 500.f;
	SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.f), FRotator(0.f, -90.f, 0.f));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.f);

	TapRange = CreateDefaultSubobject<USphereComponent>(TEXT("TAPRANGE"));
	TapRange->SetupAttachment(GetRootComponent());
	TapRange->InitSphereRadius(500.f);
	TapRange->bHiddenInGame = false;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	TapRange->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::TapSphereOnOverlapBegin);
	TapRange->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::TapSphereOnOverlapEnd);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentTargetEnemy && bIsTapOn)
	{
		FRotator LookAtYaw = GetMonsterLookRotator(CurrentTargetEnemy->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, GetWorld()->GetDeltaSeconds(), 30.f); // R은 Rotator

		SetActorRotation(InterpRotation);
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;
		SpringArm->SetRelativeRotation(LookAtYaw);
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;
	}

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMainCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Tap"), EInputEvent::IE_Pressed, this, &AMainCharacter::Tap);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMainCharacter::Yaw);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMainCharacter::Pitch);

}


void AMainCharacter::UpDown(float Value)
{
	if (IsAttacking)
		return;
	// 정면기준으로 앞으로가야하고
	const FRotator ControllerRotator = Controller->GetControlRotation();
	const FRotator RotatorYaw = FRotator(0.f, ControllerRotator.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(RotatorYaw).GetUnitAxis(EAxis::X);

	AddMovementInput(Direction, Value);
}

void AMainCharacter::LeftRight(float Value)
{
	if (IsAttacking)
		return;
	// 정면기준으로 오른쪽으로 가야하니깐
	const FRotator ControllerRotator = Controller->GetControlRotation();
	const FRotator RotatorYaw = FRotator(0.f, ControllerRotator.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(RotatorYaw).GetUnitAxis(EAxis::Y);

	AddMovementInput(Direction, Value);
}

void AMainCharacter::Yaw(float Value)
{
	AddControllerYawInput(Value);
}

void AMainCharacter::Pitch(float Value)
{
	AddControllerPitchInput(Value);
}

void AMainCharacter::Tap()
{
	if (bIsTapOn)
	{
		bIsTapOn = false;
	}
	else
	{
		bIsTapOn = true;
	}
}

void AMainCharacter::TapSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
	{
		CurrentTargetEnemy = Enemy;
	}
}
	


void AMainCharacter::TapSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsTapOn)
	{
		CurrentTargetEnemy = nullptr;
		bIsTapOn = false;
	}
}

FRotator AMainCharacter::GetMonsterLookRotator(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f); // 고개돌리기위해 Yaw값만 필요함
	return LookAtRotationYaw;
}
