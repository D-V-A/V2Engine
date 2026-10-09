#pragma once

enum class CharacterMovement
{
	Idle,
	Walking,
	Running,
	Count
};

enum class CharacterStance
{
	Standing,
	Crouching,
	Count
};

enum class CharacterCombatState
{
	Relaxed,
	Aiming,
	Count
};

enum class CharacterAction
{
	None,
	Shooting,
	Throwing,
	Reloading,
	MeleeAttack,
	UsingItem,
	Interacting,
	Hit,
	Dying,
	Count
};

enum class Direction
{
	East,
	SouthEast,
	South,
	SouthWest,
	West,
	NorthWest,
	North,
	NorthEast,
	Count
};

struct CharacterState
{
	CharacterMovement movement = CharacterMovement::Idle;
	CharacterStance stance = CharacterStance::Standing;
	CharacterCombatState combat = CharacterCombatState::Relaxed;
	CharacterAction action = CharacterAction::None;

	Direction viewDirection = Direction::South;
	Direction moveDirection = Direction::South;

	bool operator==(const CharacterState&) const = default;
};