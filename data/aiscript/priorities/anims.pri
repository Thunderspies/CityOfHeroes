//**********************************************************************
// PL_Threaten: Do threatening animation.
//**********************************************************************

PriorityList PL_Threaten
{
	Priority Threaten
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Threat"
		}

		Event PickRandomWeapon
		{
			AtTime:		0
			Condition:	random.percent < 50
			Action:		actionUseRandomPower
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}
	}
}

//**********************************************************************
// PL_AnimalThreaten: Do animalistic threatening animation.
//**********************************************************************

PriorityList PL_AnimalThreaten
{
	// TODO: Change to the correct anim when available
	Priority Threaten
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Threat"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}
	}
}

//**********************************************************************
// PL_AnimalRampage: Animalistic Vandalism
//**********************************************************************

PriorityList PL_AnimalRampage
{
	// TODO: Change to the correct anim when available
	Priority Vandalize
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Vandalize"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}
	}
}



//**********************************************************************
// PL_Eat_Junk: monster eating metal junk.
//**********************************************************************

PriorityList PL_Eat_Junk
{
	Priority Eat_Junk
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Eat", "high"
		}
	}
}



//**********************************************************************
// PL_WolfEat: Werewolf eating.
//**********************************************************************

PriorityList PL_WolfEat
{
	Priority WolfEat
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Eat"
		}
	}
}

//**********************************************************************
// PL_Howl: Werewolf howling.
//**********************************************************************

PriorityList PL_Howl
{
	Priority Howl
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Yell", "Strong"
		}
	}
}

//**********************************************************************
// PL_PipeRampage: Pick up and throw a pipe
//**********************************************************************

PriorityList PL_PipeRampage
{
	Priority Rampage
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Rampage", "Low"
		}
	}
}


//**********************************************************************
// PL_Toss_Crate: Pick up and throw a crate
//**********************************************************************

PriorityList PL_Toss_Crate
{
	Priority Toss_Crate
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"High", "Rampage", "Low"
		}
	}
}

//**********************************************************************
// PL_HammerRampage: 
//**********************************************************************

PriorityList PL_HammerRampage
{
	Priority Rampage
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Rampage", "Weapon"
		}
	}
}


//**********************************************************************
// PL_RockRampage: 
//**********************************************************************

PriorityList PL_RockRampage
{
	Priority Rampage
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Rampage", "High"
		}
	}
}


//**********************************************************************
// PL_Menace: Do threatening animation.
//**********************************************************************

PriorityList PL_Menace
{
	Priority Menace
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Menace"
		}

		Event PickRandomWeapon
		{
			AtTime:		0
			Condition:	random.percent < 50
			Action:		actionUseRandomPower
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}
	}
}

//**********************************************************************
// PL_CrazyMenace: Do threatening animation.
//**********************************************************************

PriorityList PL_CrazyMenace
{
	Priority CrazyMenace
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Menace", "Strong"
		}
	}
}

//**********************************************************************
// PL_BatMenace: Do threatening animation.
//**********************************************************************

PriorityList PL_BatMenace
{
	Priority BatMenace
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "Blunt", "Menace"
		}
	}
}

//**********************************************************************
// PL_KnifeMenace: Do threatening animation.
//**********************************************************************

PriorityList PL_KnifeMenace
{
	Priority KnifeMenace
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "Blade", "Menace"
		}
	}
}

//**********************************************************************
// PL_Cower: Do cowering animation.
//**********************************************************************

PriorityList PL_Cower
{
	Priority Cower
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Fear"
		}
	}
}

//**********************************************************************
// PL_BeGoodAndCower: Do cowering animation.
//**********************************************************************

PriorityList PL_BeGoodAndCower
{
	Priority BeGoodAndCower
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Fear"
		}

		Event Next
		{
			AtTime: 0
			Set: me.findTarget = false
			Set: me.onTeamGood = true
			Set: me.onTeamEvil = false
		}
	}
}

//**********************************************************************
// PL_Vandalize: Vandalize something.
//**********************************************************************

PriorityList PL_Vandalize
{
	Priority Vandalize
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Vandalize"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}
	}
}


//**********************************************************************
// PL_Cheering: 
//**********************************************************************

PriorityList PL_Cheering
{
	Priority Cheering
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Cheer"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}
	}
}

//**********************************************************************
// PL_WeaponCheering: 
//**********************************************************************

PriorityList PL_WeaponCheering
{
	Priority Cheering
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Cheer", "Combat", "Weapon"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"Blade", "Blunt", "Handgun"
			}
		}
	}
}

//**********************************************************************
// PL_BigWave: Waving arms and calling for help.
//**********************************************************************

PriorityList PL_BigWave
{
	Priority BigWave
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Far", "Wave", "High"
		}
	}
}

//**********************************************************************
// PL_Lookout : 
//**********************************************************************

PriorityList PL_Lookout
{
	Priority Lookout
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Lookout"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}
	}
}

//**********************************************************************
// PL_ShotgunLookout : 
//**********************************************************************

PriorityList PL_ShotgunLookout
{
	Priority ShotgunLookout
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "Shotgun", "Lookout"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}
	}
}

//**********************************************************************
// PL_PistolLookout : 
//**********************************************************************

PriorityList PL_PistolLookout
{
	Priority Lookout
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "Handgun", "Lookout"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}
	}
}

//**********************************************************************
// PL_BatLookout : 
//**********************************************************************

PriorityList PL_BatLookout
{
	Priority Lookout
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "Blunt", "Lookout"
		}

	}
}

//**********************************************************************
// PL_ShotgunKneel : 
//**********************************************************************

PriorityList PL_ShotgunKneel
{
	Priority ShotgunKneel
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Kneel", "Shotgun", "Lookout"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}
	}
}

//**********************************************************************
// PL_LookSuspicious: Look suspicious.
//**********************************************************************
//
//PriorityList PL_LookSuspicious
//{
//	Priority LookSuspicious
//	{
//		Action
//		{
//			Type:		actionDoAnim
//			AnimBits:	"Menace"
//		}
//
//		Event PickRandomWeapon
//		{
//			AtTime:		0
//			Condition:	random.percent < 50
//			Action:		actionUseRandomPower
//		}
//
//		Event AddVariationBit
//		{
//			AtTime:	0
//
//			Condition:	random.percent < 66
//
//			Action {
//				Type:		actionAddRandomAnimBit
//				AnimBits:	"VariationB", "VariationC"
//			}
//		}
//	}
//}

//**********************************************************************
// PL_Scan: Scan with medical tricorder.
//**********************************************************************

PriorityList PL_Scan
{
	Priority Scan
	{
		Action {
			Type:		actionDoAnim
			AnimBits:	"Medother", "Strong", "High"
		}
	}
}

//**********************************************************************
// PL_Clipboard: Writing on clipboard.
//**********************************************************************

PriorityList PL_ClipBoard
{
	Priority Clipboard
	{
		Action {
			Type:		actionDoAnim
			AnimBits:	"Clipboard"
		}
	}
}

//**********************************************************************
// PL_Newspaper: Reading newspaper.
//**********************************************************************

PriorityList PL_Newspaper
{
	Priority Newspaper
	{
		Action {
			Type:		actionDoAnim
			AnimBits:	"Newspaper"
		}
	}
}


//**********************************************************************
// PL_Rogue_Newspaper: Reading newspaper, but for villains (different paper)
//**********************************************************************

PriorityList PL_Rogue_Newspaper
{
	Priority Rogue_Newspaper
	{
		Action {
			Type:		actionDoAnim
			AnimBits:	"Newspaper", "Evil"
		}
	}
}

//**********************************************************************
// PL_Cellphone: Talking on phone.
//**********************************************************************

PriorityList PL_Cellphone
{
	Priority Cellphone
	{
		Action {
			Type:		actionDoAnim
			AnimBits:	"Cellphone"
		}
	}
}

//**********************************************************************
// PL_Briefcase: Idle with prop.
//**********************************************************************

PriorityList PL_Briefcase
{
	Priority Briefcase
	{
		Action {
			Type:		actionDoAnim
			AnimBits:	"Briefcase"
		}
	}
}

//**********************************************************************
// PL_Wounded: Standing looking dizzy.
//**********************************************************************

PriorityList PL_Wounded
{
	Priority Wounded
	{
		Action {
			Type:		actionDoAnim
			AnimBits:	"Stun", "Head"
		}
	}
}

//**********************************************************************
// PL_LockPick: Do lock picking animation.
//**********************************************************************

PriorityList PL_LockPick
{
	Priority LockPick
	{
		Action {
			Type:		actionDoAnim
			AnimBits:	"vandalize"
		}
	}
}

//**********************************************************************
// PL_ShootDice : Do shooting dice animation.
//**********************************************************************

//PriorityList {
//	Name: PL_ShootDice
//
//	Priority {
//		Name: ShootDice
//		
//		Action {
//			Type:		actionDoAnim
//			AnimBits:	"shootdice"
//		}
//	}
//}

//**********************************************************************
// PL_WarmHands: WArm hands over a trashcan
//**********************************************************************

PriorityList PL_WarmHands
{
	Priority Warm
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Observe"
		}
	}
}

//**********************************************************************
// PL_FlashlightObserving: Stand around looking like a punk.
//**********************************************************************

PriorityList PL_FlashlightObserving
{
	Priority FlashlightObserving
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Observe", "High"
		}
	}
}

//**********************************************************************
// PL_HunchedObserve: Stand around looking like a punk.
//**********************************************************************

PriorityList PL_HunchedObserving
{
	Priority HunchedObserving
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Observe", "Low"
		}
	}
}

//**********************************************************************
// PL_ArmsCrossed: Stand around looking like an important punk.
//**********************************************************************

PriorityList PL_ArmsCrossed
{
	Priority ArmsCrossed
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Command", "Observe", "Low"
		}
	}
}
//**********************************************************************
// PL_Observe: Stand around looking like a punk.
//**********************************************************************

PriorityList PL_Observing
{
	Priority Observing
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Observe"
		}
	}
}

//**********************************************************************
// PL_ShotgunObserve: Stand around looking like a punk with a shotgun.
//**********************************************************************

PriorityList PL_ShotgunObserve
{
	Priority ShotgunObserve
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "Shotgun", "Observe"
		}
	}
}

//**********************************************************************
// PL_HandgunObserve: Stand around looking like a punk with a handgun.
//**********************************************************************

PriorityList PL_PistolObserve
{
	Priority PistolObserve
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "Handgun", "Observe"
		}
	}
}

//**********************************************************************
// PL_Bat_Observe: Stand around looking like a punk with a bat.
//**********************************************************************

PriorityList PL_BatObserve
{
	Priority BatObserve
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "Blunt", "Observe"
		}
	}
}
//**********************************************************************
// PL_Observer: Stand around looking like a punk.
//**********************************************************************

PriorityList PL_Observer
{
	Priority Observer
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Observe"
		}
	}
}

//**********************************************************************
// PL_CastingHoldSpell: Casting a hold spell.
//**********************************************************************

PriorityList PL_CastingHoldSpell
{
	Priority Cast
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spellcast"
		}
	}
}

//**********************************************************************
// PL_FloatingStruggle: Used for NPC floating in the air, being held magically
//**********************************************************************

PriorityList PL_FloatingStruggle
{
	Priority Float
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spellcast", "Fear"
		}
	}
}


//**********************************************************************
// PL_FloatingStruggle_Hero: Used for Hero/Villain floating in the air, being held magically
//**********************************************************************

PriorityList PL_FloatingStruggle_Hero
{
	Priority FloatingStruggle_Hero
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"HeroSpellCast"
		}
	}
}


//**********************************************************************
// PL_CastingFire: Casting a hold spell.
//**********************************************************************

PriorityList PL_CastingFire
{
	Priority Cast
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spellcast"
		}
	}
}


//**********************************************************************
// PL_CastingLawRune: like PL_CastingFire only blue fx
//**********************************************************************

PriorityList PL_CastingLawRune
{
	Priority CastingLawRune
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spellcast", "sad"
		}
	}
}

//**********************************************************************
// PL_CastingPantheon: Casting a hold spell.
//**********************************************************************

PriorityList PL_CastingPantheon
{
	Priority CastingPantheon
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spellcast", "Zombie"
		}
	}
}

//**********************************************************************
// PL_Bowing
//**********************************************************************

PriorityList PL_Bowing
{
	Priority Bowing
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Bowing"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}
	}
}

//**********************************************************************
// PL_Chanting
//**********************************************************************

PriorityList PL_Chanting
{
	Priority Chanting
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Chanting"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}
	}
}

//**********************************************************************
// PL_ZombieDrum_Stand
//**********************************************************************

PriorityList PL_ZombieDrum_Standing
{
	Priority ZombieDrum_Standing
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Chanting", "High", "Wood"
		}
	}
}

//**********************************************************************
// PL_ZombieDrum_Sit
//**********************************************************************

PriorityList PL_ZombieDrum_Sitting
{
	Priority ZombieDrum_Sitting
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Chanting", "Low", "Wood"
		}
	}
}




//**********************************************************************
// PL_Meditate
//**********************************************************************

PriorityList PL_Meditate
{
	Priority Meditate
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Meditate"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}
	}
}

//**********************************************************************
// PL_Meditate_Sitting
//**********************************************************************

PriorityList PL_Meditate_Sitting
{
	Priority Meditate_Sitting
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Meditate", "Sit"
		}
	}
}



//**********************************************************************
// PL_CrateSit
//**********************************************************************

PriorityList PL_CrateSit
{
	Priority CrateSit
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Sit", "High"
		}
	}
}

//**********************************************************************
// PL_Sitting
//**********************************************************************

PriorityList PL_Sitting
{
	Priority Sitting
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Sit", "Low"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}
	}
}

//**********************************************************************
// PL_Preach: Casting a hold spell.
//**********************************************************************

PriorityList PL_Preach
{
	Priority Preach
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Speech"
		}
	}
}

//**********************************************************************
// PL_Preach_Low: Casting a hold spell.
//**********************************************************************

PriorityList PL_Preach_Low
{
	Priority Preach
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Speech", "Low"
		}
	}
}
//**********************************************************************
// PL_Listen
//**********************************************************************

PriorityList PL_Listen
{
	Priority Listen
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Speech", "Listen"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}
	}
}

//**********************************************************************
// PL_ListenHigh
//**********************************************************************

PriorityList PL_ListenHigh
{
	Priority Listen
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Speech", "Listen", "High"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}
	}
}

//**********************************************************************
// PL_ListenLow
//**********************************************************************

PriorityList PL_ListenLow
{
	Priority ListenLow
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Speech", "Listen", "Low"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}
	}
}

//**********************************************************************
// PL_DrillInstructor: 
//**********************************************************************

PriorityList PL_DrillInstructor
{
	Priority DrillInstructor
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Cheer"
		}
	}
}

//**********************************************************************
// PL_Training
//**********************************************************************

PriorityList PL_Training
{
	Priority Training
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Train", "Kungfu"
		}
	}
}

//**********************************************************************
// PL_Training_JumpingJacks 
//**********************************************************************

PriorityList PL_Training_JumpingJacks
{
	Priority Training_JumpingJacks
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Train", "Low"
		}
	}
}


//**********************************************************************
// PL_Bow_Shoot
//**********************************************************************

PriorityList PL_Bow_Shoot
{
	Priority Bow_Shoot
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Train", "Archery"
		}
	}
}

//**********************************************************************
// PL_Dealing_Deal
//**********************************************************************

PriorityList PL_Dealing_Deal
{
	Priority Dealing_Deal
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Dealing"
		}
	}
}

//**********************************************************************
// PL_Dealing_Receive
//**********************************************************************

PriorityList PL_Dealing_Receive
{
	Priority Receive
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Receiving"
		}
	}
}


//**********************************************************************
// PL_Peddling
//**********************************************************************

PriorityList PL_Peddling
{
	Priority Peddle
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Dealing"
		}
	}
}

//**********************************************************************
// PL_Repair
//**********************************************************************

PriorityList PL_Repair
{
	Priority Repair
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Repair"
		}
	}
}

//**********************************************************************
// PL_BeingRepaired
//**********************************************************************

PriorityList PL_BeingRepaired
{
	Priority BeingRepaired
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Repair", "Low"
		}
	}
}


//**********************************************************************
// PL_ClockworkSpawn
//**********************************************************************

PriorityList PL_ClockworkSpawn
{
	Priority ClockworkSpawn
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spawn", "Low", "Observe"
		}

		Event Stop
		{
			AtTime: 5
			Action: actionDisablePriority
		}
	}
	
	Priority Combat
	{
		Action: actionCombat
	}
}



//**********************************************************************
// PL_KneelingLoot
//**********************************************************************

PriorityList PL_KneelingLoot
{
	Priority KneelingLoot
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Loot"
		}
	}
}

//**********************************************************************
// PL_Dead
//**********************************************************************

PriorityList PL_Dead
{

	Priority Die
	{
		Set: me.untargetable = true

		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"hit" "death" "loot"
		}
	}
}



//**********************************************************************
// PL_DeadCleanUp
//**********************************************************************
PriorityList PL_DeadCleanUp
{
	Priority CleanUpDead
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"hit" "death" "loot"
		}

		Event AddVariationBit
		{
			AtTime:	20

			Set: me.destroy = true
		}
	}
}

//**********************************************************************
// PL_StaticObject
//**********************************************************************
PriorityList PL_StaticObject
{
	Priority StandStill
	{
		Set: me.untargetable = true
		Set: me.flying = true
		Action: actionStandStill
	}
}

//**********************************************************************
// PL_StaticObjectCleanUp
//**********************************************************************
PriorityList PL_StaticObjectCleanUp
{
	Priority CleanUp
	{
		Event AddVariationBit
		{
			AtTime:	20

			Set: me.destroy = true
		}
	}
}

//**********************************************************************
// PL_PurseTug: Do pursesnatch animation.
//**********************************************************************

PriorityList PL_PurseTug
{
	Priority PurseTug
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"PurseTug"
		}
	}
}



//**********************************************************************
// PL_ChickenTug: Do pursesnatch animation. "don't think naughty thoughts"
//**********************************************************************

PriorityList PL_ChickenTug
{
	Priority ChickenTug
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"PurseTug", "Quick"
		}
	}
}

//**********************************************************************
// PL_RelicTug: Do pursesnatch animation with a relic instead of purse.
//**********************************************************************

PriorityList PL_RelicTug
{
	Priority RelicTug
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RELICTUG"
		}
	}
}
//**********************************************************************
// PL_Rumble_FighterA: Do one side of the rumble animation.
//**********************************************************************

PriorityList PL_Rumble_FighterA
{
	Priority Rumble_FighterA
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RumbleA"
		}
	}
}

//**********************************************************************
// PL_Rumble_FighterB: Do one side of the rumble animation.
//**********************************************************************

PriorityList PL_Rumble_FighterB
{
	Priority Rumble_FighterB
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RumbleB"
		}
	}
}

//**********************************************************************
// PL_RumbleLow_FighterA: Do one side of the rumble animation.
//**********************************************************************

PriorityList PL_RumbleLow_FighterA
{
	Priority RumbleLow
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RumbleA", "Low"
		}
	}
}

//**********************************************************************
// PL_RumbleLow_FighterB: Do one side of the rumble animation.
//**********************************************************************

PriorityList PL_RumbleLow_FighterB
{
	Priority RumbleLow
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RumbleB", "Low"
		}
	}
}


//**********************************************************************
// PL_RumbleKnife_FighterA: Do one side of the rumble animation.
//**********************************************************************

PriorityList PL_RumbleKnife_FighterA
{
	Priority RumbleKnife
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RumbleA", "Knife"
		}
	}
}

//**********************************************************************
// PL_RumbleKnife_FighterB: Do one side of the rumble animation.
//**********************************************************************

PriorityList PL_RumbleKnife_FighterB
{
	Priority RumbleKnife
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RumbleB", "Knife"
		}
	}
}


//**********************************************************************
// PL_RumbleBlunt_FighterA: Do one side of the rumble animation.
//**********************************************************************

PriorityList PL_RumbleBlunt_FighterA
{
	Priority RumbleBlunt
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RumbleA", "Blunt"
		}
	}
}

//**********************************************************************
// PL_RumbleBlunt_FighterB: Do one side of the rumble animation.
//**********************************************************************

PriorityList PL_RumbleBlunt_FighterB
{
	Priority RumbleBlunt
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RumbleB", "Blunt"
		}
	}
}


//**********************************************************************
// PL_RumbleZombie_Zombie
//**********************************************************************
PriorityList PL_RumbleZombie_Zombie
{
	Priority Rumble
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RumbleB", "Magic", "Cast"
		}
	}
}


//**********************************************************************
// PL_Rumble_Knocked_Back
//**********************************************************************
PriorityList PL_Rumble_Knocked_Back
{
	Priority Rumble_Knocked_Back
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RumbleB", "Magic", "Cast"
		}
	}
}

//**********************************************************************
// PL_RumbleZombie_CotBlast
//**********************************************************************
PriorityList PL_RumbleZombie_CotBlast
{
	Priority Rumble
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RumbleA", "Magic", "Cast"
		}
	}
}

//**********************************************************************
// PL_RumbleZombie_Kick
//**********************************************************************
PriorityList PL_RumbleZombie_Kick
{
	Priority Rumble
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RumbleA", "Kick", "Attack"
		}
	}
}

//**********************************************************************
// PL_GroundPunch:
//**********************************************************************

PriorityList PL_GroundPunch
{
	Priority Punch
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"GroundPunch"
		}
	}
}

//**********************************************************************
// PL_GroundPunch_Receive:
//**********************************************************************

PriorityList PL_GroundPunch_Receive
{
	Priority Punch
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"GroundPunch", "Low"
		}
	}
}


//**********************************************************************
// PL_Shake:
//**********************************************************************

PriorityList PL_Shake
{
	Priority Punch
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Shake"
		}
	}
}

//**********************************************************************
// PL_Shake_Receive:
//**********************************************************************

PriorityList PL_Shake_Receive
{
	Priority Punch
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Shake", "Low"
		}
	}
}

//**********************************************************************
// PL_Rise_From_Ground:
//**********************************************************************

PriorityList PL_Rise_From_Ground
{
	Priority Rise_From_Ground
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spawn", "Groundsummon"
		}
	}
}


//**********************************************************************
// PL_Rise_From_Water: 
//**********************************************************************

PriorityList PL_Rise_From_Water
{
	Priority Rise_From_Water
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spawn", "Groundsummon", "Low"
		}
	}
}



//**********************************************************************
// PL_Zombie_Resurrect: Dead zombie slowly gets up from the ground.
//**********************************************************************

PriorityList PL_Zombie_Resurrect
{
	Priority Rise
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RaiseDead", "Spawn"
		}
	}
}
//**********************************************************************
// PL_Looting:
//**********************************************************************

PriorityList PL_Looting
{
	Priority Looting
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Box", "Carry", "Loot"
		}
	}
}

//**********************************************************************
// PL_Looting_Loiter: Sneaking around
//**********************************************************************

PriorityList PL_Looting_Loiter
{
	Priority LootingLoiter
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Box", "Carry", "Loot", "High"
		}
		Event Done
		{
			AtTime:		1
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		2
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		3
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		4
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 5
			Action: actionNextPriority
		}
	}
	
	Priority Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"Box", "Carry", "Loot", "High"
		}

		Event Done
		{
			AtTime:		10
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		15
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 20
			Action: actionNextPriority
		}
	}
}



//**********************************************************************
// PL_Kneeling:
//**********************************************************************

PriorityList PL_Kneeling
{
	Priority Kneeling
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Kneel"
		}
	}
}

//**********************************************************************
// PL_Kneeling_Low:
//**********************************************************************

PriorityList PL_Kneeling_Low
{
	Priority Kneeling
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Kneel", "Low"
		}
	}
}

//**********************************************************************
// PL_Kneeling_KungFu:
//**********************************************************************

PriorityList PL_Kneeling_Kungfu
{
	Priority Kneeling_Kungfu
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Kneel", "Kungfu"
		}
	}
}


//**********************************************************************
// PL_Kneeling_KungFu_Meditate:
//**********************************************************************

PriorityList PL_Kneeling_Kungfu_Meditate
{
	Priority Kneeling_Kungfu_Meditate
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Kneel", "Kungfu", "Meditate"
		}
	}
}

//**********************************************************************
// PL_Kneeling_KungFu_Low:
//**********************************************************************

PriorityList PL_Kneeling_Kungfu_Low
{
	Priority Kneeling_Kungfu_Low
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Kneel", "Low", "Kungfu"
		}
	}
}

//**********************************************************************
// PL_Saluting:
//**********************************************************************

PriorityList PL_Saluting
{
	Priority Saluting
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Salute", "High"
		}
	}
}

//**********************************************************************
// PL_Setup_Bomb: No animation, just a pose.  Intentional?
//**********************************************************************

PriorityList PL_Setup_Bomb
{
	Priority SetupBomb
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Loot", "Low"
		}
	}
}

//**********************************************************************
// PL_Shooting_Rifle_Randomized: Gunfight - Good.  Stick with randomized?
//**********************************************************************

PriorityList PL_Shooting_Rifle_Randomized
{
	Priority ShootingRifleRandomized
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Attack", "Dealing", "Rifle"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"Low", "High"
			}
		}
	}
}

//**********************************************************************
// PL_RumbleBlunt_Random: Randomizes downward bat swing, adds some bat-flipping, and a little chest beating.
//**********************************************************************

PriorityList PL_RumbleBlunt_Random
{
	Priority RumbleBlunt
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RumbleA", "Blunt"
		}
		Event Done
		{
			AtTime:		1
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		2
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		3
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		4
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 5
			Action: actionNextPriority
		}
	}
	
	Priority BatMenace
	{		
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "Blunt", "Menace"
		}

		Event Done
		{
			AtTime:		10
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		15
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 20
			Action: actionNextPriority
		}
	}

	Priority CrazyMenace
	{		
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Menace", "Strong"
		}

		Event Done
		{
			AtTime:		10
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		15
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 20
			Action: actionNextPriority
		}
	}
}

//**********************************************************************
// PL_Shooting_Rifle_Normal:
//**********************************************************************

PriorityList PL_Shooting_Rifle_Normal
{
	Priority ShootingRifleNormal
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Attack", "Dealing", "Rifle"
		}
	}
}

//**********************************************************************
// PL_Shooting_Rifle_High:
//**********************************************************************

PriorityList PL_Shooting_Rifle_High
{
	Priority ShootingRifleHigh
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Attack", "Dealing", "Rifle", "High"
		}
	}
}

//**********************************************************************
// PL_Shooting_Rifle_Low:
//**********************************************************************

PriorityList PL_Shooting_Rifle_Low
{
	Priority ShootingRifleLow
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Attack", "Dealing", "Rifle", "Low"
		}
	}
}

//**********************************************************************
// PL_Rumble_Sword: No sword appears.  Are we waiting on the prop?
//**********************************************************************

PriorityList PL_Rumble_Sword_B
{
	Priority RumbleSword
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Sword", "RumbleB", "Low"
		}
	}
}

//**********************************************************************
// PL_Rumble_Plantman_Sword:
//**********************************************************************

PriorityList PL_Rumble_Plantman_Sword
{
	Priority RumblePlantmanSword
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RumbleA", "Low"
		}
	}
}

//**********************************************************************
// PL_Plantman_Ambush:  
//**********************************************************************

PriorityList PL_Plantman_Ambush
{
	Priority PlantmanAmbush
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spawn", "High"
		}
	}
}

//**********************************************************************
// PL_Plantman_RampageA:
//**********************************************************************

PriorityList PL_Plantman_RampageA
{
	Priority PlantmanRampageA
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Rampage", "High"
		}
	}
}

//**********************************************************************
// PL_Plantman_RampageB: 
//**********************************************************************

PriorityList PL_Plantman_RampageB
{
	Priority PlantmanRampageB
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Rampage", "Low"
		}
	}
}

//**********************************************************************
// PL_Rockman_Ambush: 
//**********************************************************************

PriorityList PL_Rockman_Ambush
{
	Priority RockmanAmbush
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spawn", "Low", "high"
		}
	}
}


//**********************************************************************
// PL_WarmHands_NoBarrel: Warming hands with no barrel
//**********************************************************************

PriorityList PL_WarmHands_NoBarrel
{
	Priority WarmHandsNoBarrel
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Hand", "Loiter"
		}
	}
}

//**********************************************************************
// PL_Teleport_Clockwork: Clockwork Teleport FX
//**********************************************************************

PriorityList PL_Teleport_Clockwork
{
	Priority TeleportClockwork
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spawn", "Teleport"
		}

		Set: me.invisible = true
	}
}

//**********************************************************************
// PL_Shotgun_Alert:
//**********************************************************************

PriorityList PL_Shotgun_Alert
{
	Priority Shotgun_Alert
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Shotgun", "Pose", "Mid"
		}
	}
}


//**********************************************************************
// PL_Shotgun_Point:
//**********************************************************************

PriorityList PL_Shotgun_Point
{
	Priority Shotgun_Point
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Shotgun", "Pose", "Point", "High"
		}
	}
}

//**********************************************************************
// PL_Crossbow_Threaten: pointing crossbow at victim
//**********************************************************************

PriorityList PL_Crossbow_Threaten
{
	Priority Crossbow_Threaten
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Crossbow", "Pose", "Point", "High"
		}
	}
}


//**********************************************************************
// PL_Shotgun_Rollout_Left:
//**********************************************************************

PriorityList PL_Shotgun_Rollout_Left
{
	Priority Shotgun_Rollout_Left
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Shotgun", "Pose", "Lookout", "Left"
		}
	}
}

//**********************************************************************
// PL_Shotgun_Rollout_Right:
//**********************************************************************

PriorityList PL_Shotgun_Rollout_Right
{
	Priority Shotgun_Rollout_Right
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Shotgun", "Pose", "Lookout", "Right"
		}
	}
}

//**********************************************************************
// PL_Shotgun_Duck_Shoot:
//**********************************************************************

PriorityList PL_Shotgun_Duck_Shoot
{
	Priority Shotgun_Duck_Shoot
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Shotgun", "Low", "Hide", "Shoot"
		}
	}
}


//**********************************************************************
// PL_Pistol_Duck_Shoot:
//**********************************************************************

PriorityList PL_Pistol_Duck_Shoot
{
	Priority Pistol_Duck_Shoot
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Gun", "Low", "Hide", "Shoot"
		}
	}
}




//**********************************************************************
// PL_Hostage:
//**********************************************************************

PriorityList PL_Hostage
{
	Priority Hostage
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Fear", "Pose", "High"
		}
	}
}


//**********************************************************************
// PL_WallSneak:
//**********************************************************************

PriorityList PL_WallSneak
{
	Priority WallSneak
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Sneak", "Pose", "LookOut"
		}
	}
}


//**********************************************************************
// PL_WallPeek_Right:
//**********************************************************************

PriorityList PL_WallPeek_Right
{
	Priority WallPeek_Right
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Sneak", "Pose", "LookOut", "Right"
		}
	}
}



//**********************************************************************
// PL_WallPeek_Left:
//**********************************************************************

PriorityList PL_WallPeek_Left
{
	Priority WallPeek_Left
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Sneak", "Pose", "LookOut", "Left"
		}
	}
}


//**********************************************************************
// PL_Wall_Lean:
//**********************************************************************

PriorityList PL_Wall_Lean
{
	Priority Wall_Lean
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Angry", "threat", "Menace", "High", "Pose"
		}
	}
}


//**********************************************************************
// PL_Sword_Vandalize:
//**********************************************************************

PriorityList PL_Sword_Vandalize
{
	Priority Sword_Vandalize
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "Blade", "Vandalize"
		}
	}
}


//**********************************************************************
// PL_Teleport_Delay_30:
//**********************************************************************

PriorityList PL_Teleport_Delay_30
{
	Priority Teleport_Delay_30
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spawn", "Teleport", "Wait", "Low"
		}
	}
}

//**********************************************************************
// PL_Teleport_Delay_90:
//**********************************************************************

PriorityList PL_Teleport_Delay_90
{
	Priority Teleport_Delay_90
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spawn", "Teleport", "Wait", "Mid"
		}
	}
}

//**********************************************************************
// PL_Teleport_Delay_120:
//**********************************************************************

PriorityList PL_Teleport_Delay_120
{
	Priority Teleport_Delay_120
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spawn", "Teleport", "Wait", "High"
		}
	}
}



//**********************************************************************
// PL_DanceRave:
//**********************************************************************

PriorityList PL_DanceRave
{
	Priority DanceRave
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Dance", "Low", "Pose"
		}
	}
}


//**********************************************************************
// PL_Handcart_Loiter:
//**********************************************************************

PriorityList PL_Handcart_Loiter
{
	Priority HandcartLoiter
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Loiter", "Prop", "Low"
		}
		Event Done
		{
			AtTime:		1
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		2
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		3
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		4
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 5
			Action: actionNextPriority
		}
	}
	
	Priority Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"Loiter", "Prop", "Low"
		}

		Event Done
		{
			AtTime:		10
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		15
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 20
			Action: actionNextPriority
		}
	}
}


//**********************************************************************
// PL_DigHole:
//**********************************************************************

PriorityList PL_DigHole
{
	Priority DigHole
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Work", "Groundsummon", "Low", "Mid"
		}
	}
}


//**********************************************************************
// PL_DigShovel:
//**********************************************************************

PriorityList PL_DigShovel
{
	Priority DigShovel
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Work", "Groundsummon", "Low"
		}
	}
}




//**********************************************************************
// PL_DigPickAxe:
//**********************************************************************

PriorityList PL_DigPickAxe
{
	Priority DigPickAxe
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Work", "Groundsummon", "High"
		}
	}
}


//**********************************************************************
// PL_DigHands:
//**********************************************************************

PriorityList PL_DigHands
{
	Priority DigHands
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Work", "Hand", "Strong"
		}
	}
}


//**********************************************************************
// PL_ReadInscription:
//**********************************************************************

PriorityList PL_ReadInscription
{
	Priority ReadInscription
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Lookout", "Work", "Train"
		}
	}
}


//**********************************************************************
// PL_Book_Research: like PL_ReadInscription w/ out prop
//**********************************************************************

PriorityList PL_Book_Research
{
	Priority Book_Research
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Lookout", "Work", "Train", "Mid"
		}
	}
}


//**********************************************************************
// PL_Book_Research_Low: like PL_ReadInscription w/ out prop, Kneeling
//**********************************************************************

PriorityList PL_Book_Research_Low
{
	Priority Book_Research_Low
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Lookout", "Work", "Train", "Low"
		}
	}
}

//**********************************************************************
// PL_ReadScroll:
//**********************************************************************

PriorityList PL_ReadScroll
{
	Priority ReadScroll
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Lookout", "Work", "Train", "Wait"
		}
	}
}



//**********************************************************************
// PL_HoldTorch:
//**********************************************************************

PriorityList PL_HoldTorch
{
	Priority HoldTorch
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Lookout", "Work", "Wait", "Pose"
		}
	}
}


//**********************************************************************
// PL_FloatBooks:
//**********************************************************************

PriorityList PL_FloatBooks
{
	Priority FloatBooks
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Lookout", "Work", "Box", "Body"
		}
	}
}


//**********************************************************************
// PL_WheelBarrel_Wander:
//**********************************************************************

PriorityList PL_WheelBarrel_Wander
{

	Priority Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"Loiter", "Prop", "Work", "High"
		}

	}
}


//**********************************************************************
// PL_WheelBarrel_Wander_Pumpkins:
//**********************************************************************

PriorityList PL_WheelBarrel_Wander_Pumpkins
{

	Priority Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"Loiter", "Prop", "Work", "High", "Quick"
		}

	}
}

//**********************************************************************
// PL_BarrelRoll_Wander:
//**********************************************************************

PriorityList PL_BarrelRoll_Wander
{

	Priority Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"Loiter", "Prop", "Work"
		}

	}
}


//**********************************************************************
// PL_Greedy:
//**********************************************************************

PriorityList PL_Greedy
{
	Priority Greedy
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Observe", "Low"
		}
	}
}


//**********************************************************************
// PL_Shield_Block:
//**********************************************************************

PriorityList PL_Shield_Block
{
	Priority Shield_Block
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Shield", "Pose", "Work"
		}
	}
}

//**********************************************************************
// PL_DieIn60Sec - Sets an entity to die off after a set amount of time. Use for untargetable villains, such as CoT crystals.
//**********************************************************************

PriorityList PL_DieIn60Sec
{
	Priority Die
	{
		Event Die
		{
			Set: me.Destroy = true
			attime: 60
		}
	}
}


//**********************************************************************
// PL_Burte_WallSmash:
//**********************************************************************

PriorityList PL_Burte_WallSmash
{
	Priority Burte_WallSmash
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Vandalize", "Wall", "High"
		}
	}
}

//**********************************************************************
// PL_Burte_Barrage:
//**********************************************************************

PriorityList PL_Burte_Barrage
{
	Priority Burte_Barrage
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Vandalize", "Eye", "High"
		}
	}
}


//**********************************************************************
// PL_Burte_Eyeshoot:
//**********************************************************************

PriorityList PL_Burte_Eyeshoot
{
	Priority Burte_Eyeshoot
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Vandalize", "Eye", "Low"
		}
	}
}


//**********************************************************************
// PL_Natterling_Roof_BreakIn:
//**********************************************************************

PriorityList PL_Natterling_Roof_BreakIn
{
	Priority Natterling_Roof_BreakIn
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Vandalize", "Mid", "Throwknife"
		}
	}
}

//**********************************************************************
// PL_At_Ease:
//**********************************************************************

PriorityList PL_At_Ease
{
	Priority At_Ease
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Salute", "High", "Low", "Pose"
		}
	}
}


//**********************************************************************
// PL_Lavaman_Popup_Static:
//**********************************************************************

PriorityList PL_Lavaman_Popup_Static
{
	Priority Lavaman_Popup_Static
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Sleep", "Sneak", "Launch", "Low"
		}
	}
}





//**********************************************************************
// PL_Lavaman_Popup_Short:
//**********************************************************************

PriorityList PL_Lavaman_Popup_Short
{
	Priority PL_Lavaman_Popup_Short
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Sleep", "Sneak", "Launch", "Mid"
		}
	}
}


//**********************************************************************
// PL_Lavaman_Popup_Long:
//**********************************************************************

PriorityList PL_Lavaman_Popup_Long
{
	Priority Lavaman_Popup_Long
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Sleep", "Sneak", "Launch", "High"
		}
	}
}

//**********************************************************************
// PL_Rularuu_Angry:
//**********************************************************************

PriorityList PL_Rularuu_Angry
{
	Priority Rularuu_Angry
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Dealing", "Angry"
		}
	}
}


//**********************************************************************
// PL_DeadSlab:
//**********************************************************************

PriorityList PL_DeadSlab
{
	Priority DeadSlab
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"hit", "death", "loot", "High"
		}
	}
}

//**********************************************************************
// PL_Protest: w/ sign prop
//**********************************************************************

PriorityList PL_Protest
{
	Priority Protest
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"Angry", "Yell", "Speech", "No", "Scream"
		}
	}
}


//**********************************************************************
// PL_Pipe_Angry
//**********************************************************************

PriorityList PL_Pipe_Angry
{
	Priority Pipe_Angry
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"Angry", "Yell", "Speech", "No", "Scream", "Blunt"
		}
	}
}



//**********************************************************************
// PL_Binocular_scan:
//**********************************************************************

PriorityList PL_Binocular_scan
{
	Priority Binocular_scan
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"Lookout", "Pose", "Far"
		}
	}
}


//**********************************************************************
// PL_Crate_hold:  just standing
//**********************************************************************

PriorityList PL_Crate_hold
{
	Priority Crate_hold
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"Carry", "Box", "Loiter"
		}
	}
}


//**********************************************************************
// PL_Crate_Loiter: Stand around & Moving
//**********************************************************************

PriorityList PL_Crate_Loiter
{
	Priority WaitForAWhile
	{
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Carry", "Box", "Loiter"
		}

		Event Done
		{
			AtTime:		1
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		2
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		3
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		4
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 5
			Action: actionNextPriority
		}
	}
	
	Priority Crate_Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"Carry", "Box", "Loiter"
		}

		Event Done
		{
			AtTime:		10
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		15
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 20
			Action: actionNextPriority
		}
	}
}


//**********************************************************************
// PL_Crate_Wander: Just moving, constant motion
//**********************************************************************

PriorityList PL_Crate_Wander
{
	
	Priority Crate_Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			TargetType:	targetSpawnPoint
			AnimBits:	"Carry", "Box", "Loiter"
		}

	}
}



//**********************************************************************
// PL_WanderDynamite: Just moving, constant motion
//**********************************************************************

PriorityList PL_WanderDynamite
{
	
	Priority WanderDynamite
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			TargetType:	targetSpawnPoint
			AnimBits:	"Carry", "Box", "Loiter", "high"
		}

	}
}



//**********************************************************************
// PL_LootWander: Just moving, constant motion
//**********************************************************************

PriorityList PL_LootWander
{
	
	Priority LootWander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			TargetType:	targetSpawnPoint
			AnimBits:	"Carry", "Box", "Loiter", "Low"
		}

	}
}





//**********************************************************************
// PL_WalkieTalkie: Talking on WalkieTalkie.
//**********************************************************************

PriorityList PL_WalkieTalkie
{
	Priority WalkieTalkie
	{
		Action {
			Type:		actionDoAnim
			AnimBits:	"Chat", "Pose", "Far"
		}
	}
}



//**********************************************************************
// PL_Drill_March: Marching in Place.
//**********************************************************************

PriorityList PL_Drill_March
{
	Priority Drill_March
	{
		Action {
			Type:		actionDoAnim
			AnimBits:	"Busy", "Train", "Stomp"
		}
	}
}


//**********************************************************************
// PL_Drill_Yell: Goes with PL_Drill_March.
//**********************************************************************

PriorityList PL_Drill_Yell
{
	Priority Drill_Yell
	{
		Action {
			Type:		actionDoAnim
			AnimBits:	"Busy", "Train", "Stomp", "Yell"
		}
	}
}



//**********************************************************************
// PL_Air2Ground_Wander: Flying shooting down at Ground
//**********************************************************************

PriorityList PL_Air2Ground_Wander
{
	
	Priority Air2Ground_Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			TargetType:	targetSpawnPoint
			AnimBits:	"Loiter", "Attack", "High", "Low"
		}

	}
}



//**********************************************************************
// PL_Air2Air_Wander: Flying shooting at Air Targets
//**********************************************************************

PriorityList PL_Air2Air_Wander
{
	
	Priority Air2Air_Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			TargetType:	targetSpawnPoint
			AnimBits:	"Loiter", "Attack", "High"
		}

	}
}


//**********************************************************************
// PL_Air2Air_Wander_Skiff: Flying shooting at Air Targets
//**********************************************************************

PriorityList PL_Air2Air_Wander_Skiff
{
	
	Priority Air2Air_Wander_Skiff
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			TargetType:	targetSpawnPoint
			AnimBits:	"Loiter", "Attack", "High"
		}

	}
}



//**********************************************************************
// PL_Ground2Air: Standing Shooting in air
//**********************************************************************

PriorityList PL_Ground2Air
{
	
	Priority Ground2Air
	{		
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Loiter", "Attack", "High", "Low", "Mid"
		}

	}
}


//**********************************************************************
// PL_Vampire_Stand: Standing w/ arms crossed
//**********************************************************************

PriorityList PL_Vampire_Stand
{
	
	Priority Vampire_Stand
	{		
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Busy", "Pose", "Magic"
		}

	}
}


//**********************************************************************
// PL_Vampire_Crouch: 
//**********************************************************************

PriorityList PL_Vampire_Crouch
{
	
	Priority Vampire_Crouch
	{		
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Busy", "Pose", "Magic", "Low"
		}

	}
}


//**********************************************************************
// PL_Ape_Crouch: 
//**********************************************************************

PriorityList PL_Ape_Crouch
{
	
	Priority Ape_Crouch
	{		
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Busy", "Pose", "Magic", "Low", "Strong"
		}

	}
}


//**********************************************************************
// PL_Counter: will be typing on counter
//**********************************************************************

PriorityList PL_Counter
{
	
	Priority Counter
	{		
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"observe", "Low", "Prop", "Work"
			
		}

	}
}

//**********************************************************************
// PL_Welding_Mid: 
//**********************************************************************

PriorityList PL_Welding_Mid
{
	
	Priority Welding_Mid
	{		
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Work", "Pose", "Welding", "Mid"
		}

	}
}


//**********************************************************************
// PL_Welding_Low: 
//**********************************************************************

PriorityList PL_Welding_Low
{
	
	Priority Welding_Low
	{		
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Work", "Pose", "Welding", "Low"
		}

	}
}


//**********************************************************************
// PL_Clean_Gun: 
//**********************************************************************

PriorityList PL_Clean_Gun
{
	
	Priority Clean_Gun
	{		
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Work", "Pose", "Weapon", "Repair"
		}

	}
}


//**********************************************************************
// PL_Robot_off: 
//**********************************************************************

PriorityList PL_Robot_off
{
	
	Priority Robot_off
	{		
		HoldPriority: true
		set: me.untargetable = TRUE
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Quick", "Spawn", "Sleep"
		}

	}
}

//**********************************************************************
// PL_Robot_awake: 
//**********************************************************************

PriorityList PL_Robot_awake
{
	
	Priority Robot_awake
	{		
		set: me.untargetable = TRUE
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Quick", "Spawn", "Sleep", "No"
		}

		Event Done
		{
			AtTime: 1
			Action: actionDisablePriority
		}
	}

	Priority Combat
	{
		set: me.untargetable = FALSE
		Action: actionCombat
	}
}




//**********************************************************************
// PL_RatchetFix_Mid: 
//**********************************************************************

PriorityList PL_RatchetFix_Mid
{
	
	Priority RatchetFix_Mid
	{		
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Work", "Pose", "Club", "Mid"
		}

	}
}


//**********************************************************************
// PL_OCTOPUS_SUBMERGE: 
//**********************************************************************
PriorityList PL_OCTOPUS_SUBMERGE
{
	Priority Sink
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Pose", "Low"
		}
	}
}

//**********************************************************************
// PL_OCTOPUS_SUBMERGE: 
//**********************************************************************
PriorityList PL_OCTOPUS_RISE
{

	//Not combat???
	Priority StandStill
	{
		Action: actionStandStill
	}
}

//**********************************************************************
// PL_BoomBox: 
//**********************************************************************

PriorityList PL_BoomBox
{
	
	Priority BoomBox
	{		
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Box", "Strong", "Shuriken"
		}

	}
}

//**********************************************************************
// Bookie: 
//**********************************************************************

PriorityList PL_Bookie
{
	Priority Bookie
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Clipboard", "OBSERVE", "PROP", "High"
		}
	}
}

//**********************************************************************
// CastSpell: 
//**********************************************************************

PriorityList PL_CastSpell
{
	Priority CastSpell
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Train", "Low"
		}
	}
}


//**********************************************************************
// Breakout: 
//**********************************************************************

PriorityList PL_Breakout
{
	Priority Breakout
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Train", "Low"
		}
	}
}


//**********************************************************************
// FishAmbush: 
//**********************************************************************

PriorityList PL_FishAmbush
{
	Priority FishAmbush
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spawn", "Wait", "Low", "High" 
		}
	}
}

//**********************************************************************
// Flirt: //femine charm - blows kisses, waves & sexy poses (not hooker)
//**********************************************************************

PriorityList PL_Flirt
{
	Priority Flirt
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Train", "Low"
		}
	}
}


//**********************************************************************
// HandsUp: //captured crook style
//**********************************************************************

PriorityList PL_HandsUp
{
	Priority HandsUp
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"2hand", "Stop", "Threat", "hypnotize"
		}
	}
}


//**********************************************************************
// HandsUp_Kneel: //captured crook style
//**********************************************************************

PriorityList PL_HandsUp_Kneel
{
	Priority HandsUp_Kneel
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"2hand", "Stop", "Threat", "hypnotize" "low"
		}
	}
}


//**********************************************************************
// OilBlaze: 
//**********************************************************************

PriorityList PL_OilBlaze
{
	Priority OilBlaze
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Prop", "Vandalize", "Lookout"
		}
	}
}


//**********************************************************************
// Fire_Menace: 
//**********************************************************************

PriorityList PL_Fire_Menace
{
	Priority Fire_Menace
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Prop", "Vandalize", "Lookout", "Menace"
		}
	}
}

//**********************************************************************
// PL_Aerial_Search: Skiff doing a patrol w/ a serch light FX (works @ night)
//**********************************************************************

PriorityList PL_Aerial_Search
{
	Priority Aerial_Search
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"LOOKOUT"
		}

	}
}



//**********************************************************************
// PL_Patrol_Torch: 
//**********************************************************************

PriorityList PL_Patrol_Torch
{
	Priority Patrol_Torch
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"Lookout", "Work", "Wait", "Pose"
		}

	}
}


//**********************************************************************
// PL_Patrol_Pickaxe: 
//**********************************************************************

PriorityList PL_Patrol_Pickaxe
{
	Priority Patrol_Pickaxe
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"Lookout", "Work", "Wait", "Pose", "Groundsummon", "High"
		}

	}
}



//**********************************************************************
// Patrol_Shotgun: 
//**********************************************************************

PriorityList PL_Patrol_Shotgun
{
	Priority Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"COMBAT", "WEAPON", "SHOTGUN", "LOOKOUT"
		}

	}
}


//**********************************************************************
// Patrol_Trunchoen: 
//**********************************************************************

PriorityList PL_Patrol_Trunchoen
{
	Priority Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"COMBAT", "WEAPON", "BLUNT", "LOOKOUT", "HIGH"
		}

	}
}


//**********************************************************************
// PL_Patrol_Pipe: 
//**********************************************************************

PriorityList PL_Patrol_Pipe
{
	Priority Patrol_Pipe
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"COMBAT", "WEAPON", "BLUNT", "LOOKOUT", "HIGH", "2hand"
		}

	}
}





//**********************************************************************
// PL_Vendor:
//**********************************************************************

PriorityList PL_Vendor
{
	Priority Vendor
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Observe", "Loiter", "Busy"
		}
	}
}


//**********************************************************************
// PL_Vendor_No_Cart:
//**********************************************************************

PriorityList PL_Vendor_No_Cart
{
	Priority Vendor_No_Cart
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"NOCARTVENDOR"
		}
	}
}






//**********************************************************************
// PL_Cement_Shoes:
//**********************************************************************

PriorityList PL_Cement_Shoes
{
	Priority Cement_Shoes
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"CEMENTSHOES"
		}
	}
}



//**********************************************************************
// PL_Cement_Shoes_Wander:
//**********************************************************************

PriorityList PL_Cement_Shoes_Wander
{

	Priority Cement_Shoes_Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"CEMENTSHOES"
		}

	}
}







//**********************************************************************
// PL_Follow_Route_Run_Woonded:
//**********************************************************************

PriorityList PL_Follow_Route_Run_Woonded
{
	Priority Follow_Route_Run_Woonded
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Limp"
		}
	}
}





//**********************************************************************
// PL_HandsOnHead:
//**********************************************************************

PriorityList PL_HandsOnHead
{
	Priority HandsOnHead
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"2hand", "Stop", "Threat", "hypnotize", "head"
		}
	}
}



//**********************************************************************
// PL_HandsOnHead_Kneel:
//**********************************************************************

PriorityList PL_HandsOnHead_Kneel
{
	Priority HandsOnHead_Kneel
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"2hand", "Stop", "Threat", "hypnotize", "head", "low"
		}
	}
}


//**********************************************************************
// PL_LedgeSit:
//**********************************************************************

PriorityList PL_LedgeSit
{
	Priority LedgeSit
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Sit", "threat", "Menace", "High", "Pose"
		}
	}
}


//**********************************************************************
// PL_Chair_Sit:
//**********************************************************************

PriorityList PL_Chair_Sit
{
	Priority Chair_Sit
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Sit", "threat", "Menace", "High", "Pose", "armchair"
		}
	}
}






//**********************************************************************
// PL_Prone:
//**********************************************************************

PriorityList PL_Prone
{
	Priority Prone
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Pose", "Sad", "Head", "zombie" 
		}
	}
}



//**********************************************************************
// PL_Prone_Wander: 
//**********************************************************************

PriorityList PL_Prone_Wander
{
	Priority WaitForAWhile_Prone
	{
		HoldPriority: true

		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Yell", "Sad", "Head", "zombie", "High", "laugh"
		}

		Event Done
		{
			AtTime:		1
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		2
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		3
			Condition:	random.percent < 10
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 4
			Action: actionNextPriority
		}
	}
	
	Priority Wander_Prone	
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			TargetType:	targetSpawnPoint
			AnimBits:	"Yell", "Sad", "Head", "zombie", "High", "laugh"
		}

		Event Done
		{
			AtTime:		10
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime:		15
			Condition:	random.percent < 50
			Action:		actionNextPriority
		}

		Event Done
		{
			AtTime: 20
			Action: actionNextPriority
		}
	}
}



//**********************************************************************
// PL_Soul_Give:
//**********************************************************************

PriorityList PL_Soul_Give
{
	Priority Soul_Give
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Pose", "Sad", "Head", "zombie", "Quick"
		}
	}
}


//**********************************************************************
// PL_Soul_Take:
//**********************************************************************

PriorityList PL_Soul_Take
{
	Priority Soul_Take
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Train", "Low", "Idle"
		}
	}
}


//**********************************************************************
// PL_Sword_Point:
//**********************************************************************

PriorityList PL_Sword_Point
{
	Priority Sword_Point
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"combat", "Weapon", "sword", "Point", "Strong"
		}
	}
}



//**********************************************************************
// PL_Sword_Alert:
//**********************************************************************

PriorityList PL_Sword_Alert
{
	Priority Sword_Alert
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"sword", "pose", "mid"
		}
	}
}


//**********************************************************************
// PL_Sword_Guard:
//**********************************************************************

PriorityList PL_Sword_Guard
{
	Priority Sword_Guard
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"combat", "Weapon", "sword", "lookout", "high", "low"
		}
	}
}



//**********************************************************************
// PL_Archer_Lookout:
//**********************************************************************

PriorityList PL_Archer_Lookout
{
	Priority Archer_Lookout
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "dual", "rev", "lookout"
		}
	}
}


//**********************************************************************
// PL_Sword_Lookout:
//**********************************************************************

PriorityList PL_Sword_Lookout
{
	Priority Sword_Lookout
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "sword", "Lookout", "High"
		}
	}
}


//**********************************************************************
// PL_Sword_Observe:
//**********************************************************************

PriorityList PL_Sword_Observe
{
	Priority Sword_Observe
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "sword", "Observe", "High"
		}
	}
}







//**********************************************************************
// PL_Trunchoen_Alert:
//**********************************************************************

PriorityList PL_Trunchoen_Alert
{
	Priority Trunchoen_Alert
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "Blunt"
		}
	}
}



//**********************************************************************
// PL_Trunchoen_Guard:  //I'm using the lookout anim here instead
//**********************************************************************

PriorityList PL_Trunchoen_Guard
{
	Priority Trunchoen_Guard
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "Blunt", "Lookout", "High"
		}
	}
}



//**********************************************************************
// PL_Trunchoen_Lookout: //I'm using a couple of menace moves here
//**********************************************************************

PriorityList PL_Trunchoen_Lookout
{
	Priority Trunchoen_Lookout
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "Blunt", "Menace", "High"

		}
	}
}


//**********************************************************************
// PL_Trunchoen_Threaten:
//**********************************************************************

PriorityList PL_Trunchoen_Threaten
{
	Priority Trunchoen_Threaten
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "Blunt", "threat", "High"
		}
	}
}



//**********************************************************************
// PL_Melee_Fists:
//**********************************************************************

PriorityList PL_Melee_Fists
{
	Priority Melee_Fists
	{
		IAmA: Melee_Fists
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Punch", "RumbleA"
		}
	}
}


//**********************************************************************
// PL_Melee_Fists_react:
//**********************************************************************

PriorityList PL_Melee_Fists_react
{
	Priority Melee_Fists_react
	{
		IReactTo: Melee_Fists
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Punch", "RumbleA", "Low"
		}
	}
}


//**********************************************************************
// PL_Float_Wander:
//**********************************************************************

PriorityList PL_Float_Wander
{

	Priority Float_Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"Fly", "Hover"
		}

	}
}

//**********************************************************************
// PL_Float:
//**********************************************************************

PriorityList PL_Float
{

	Priority Float
	{		
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Fly", "Hover"
		}

	}
}

//**********************************************************************
// PL_Plantman_Scarecrow:
//**********************************************************************

PriorityList PL_Plantman_Scarecrow
{
	Priority Plantman_Scarecrow
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"Spawn", "Magic", "High"
		}
	}
}


//**********************************************************************
// PL_Plant_Trap:
//**********************************************************************

PriorityList PL_Plant_Trap
{
	Priority Plant_Trap
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"ENCOUNTERPLANTTRAP"
		}
	}
}


//**********************************************************************
// PL_Plantman_Pumpkin_Ambush:
//**********************************************************************

PriorityList PL_Plantman_Pumpkin_Ambush
{
	Priority Plantman_Pumpkin_Ambush
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spawn", "Magic", "Low"
		}
	}
}


//**********************************************************************
// PL_Lift_Totem:
//**********************************************************************

PriorityList PL_Lift_Totem
{
	Priority Lift_Totem
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Work", "2Hand", "Low", "Mid"
		}
	}
}


//**********************************************************************
// PL_TribalDance_Wander:
//**********************************************************************

PriorityList PL_TribalDance_Wander
{

	Priority Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"Dance", "Strong", "Chanting"
		}

	}
}


//**********************************************************************
// PL_Statesman_Shackled:
//**********************************************************************

PriorityList PL_Statesman_Shackled
{
	Priority Statesman_Shackled
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Cage", "Force", "Evil", "Low", "High"
		}
	}
}


//**********************************************************************
// PL_Torch_Wander:
//**********************************************************************

PriorityList PL_Torch_Wander
{

	Priority Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"Lookout", "Work", "Wait", "Pose"
		}

	}
}


//**********************************************************************
// PL_Book_Wander:
//**********************************************************************

PriorityList PL_Book_Wander
{

	Priority Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"Box", "Plan", "Pose"
		}

	}
}


//**********************************************************************
// PL_Magic_Wander:
//**********************************************************************

PriorityList PL_Magic_Wander
{

	Priority Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"Loiter", "Magic"
		}

	}
}



//**********************************************************************
// PL_Play_Dice:
//**********************************************************************

PriorityList PL_Play_Dice
{
	Priority Play_Dice
	{
		IAmA: DiceRoller
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Encounter", "Pose", "RollDice", "High"
		}
	}
}

//**********************************************************************
// PL_Play_Dice_Observe:
//**********************************************************************

PriorityList PL_Play_Dice_Observe
{
	Priority Play_Dice_Observe
	{
		IReactTo: DiceRoller
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"LOW", "OBSERVE"
		}

		Event AddVariationBit
		{
			AtTime:	0

			Condition:	random.percent < 66

			Action
			{
				Type:		actionAddRandomAnimBit
				AnimBits:	"VariationB", "VariationC"
			}
		}
	}
}

//**********************************************************************
// PL_Surrounded
//**********************************************************************

PriorityList PL_Surrounded
{
	Priority Surrounded
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Strong", "Menace", "Lookout"
		}
	}
}



//**********************************************************************
// PL_Casting_Shield
//**********************************************************************

PriorityList PL_Casting_Shield
{
	Priority Casting_Shield
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Shield", "Pose", "Magic", "Cast"
		}
	}
}


//**********************************************************************
// PL_Hide_N_Seek
//**********************************************************************

PriorityList PL_Hide_N_Seek
{
	Priority Hide_N_Seek
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Hide", "Lookout", "Spawn"
		}
	}
}


//**********************************************************************
// PL_Jeering
//**********************************************************************

PriorityList PL_Jeering
{
	Priority Jeering
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Angry", "TauntA", "TauntB", "TauntC"
		}
	}
}


//**********************************************************************
// PL_FireMarshall_Backpack
//**********************************************************************

PriorityList PL_FireMarshall_Backpack
{
	Priority FireMarshall_Backpack
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Work", "Fire", "Backpack", "Help"
		}
	}
}

//**********************************************************************
// PL_Cage_Croatoa
//**********************************************************************

PriorityList PL_Cage_Croatoa
{
	Priority Cage_Croatoa
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Fear", "Cage", "Low"
		}
	}
}


//**********************************************************************
// PL_Ambush_Shiva
//**********************************************************************

PriorityList PL_Ambush_Shiva
{
	Priority Ambush_Shiva
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Low", "Spawn", "Wait"
		}
	}
}


//**********************************************************************
// PL_SprayPaint
//**********************************************************************

PriorityList PL_SprayPaint
{
	Priority SprayPaint
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spraycan"
		}
	}
}

//**********************************************************************
// PL_PushAround_A
//**********************************************************************

PriorityList PL_PushAround_A
{
	Priority PushAround_A
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Push", "2Hand", "High"
		}
	}
}

//**********************************************************************
// PL_PushAround_B
//**********************************************************************

PriorityList PL_PushAround_B
{
	Priority PushAround_B
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Push", "2Hand", "Mid"
		}
	}
}

//**********************************************************************
// PL_PushAround_Victim
//**********************************************************************

PriorityList PL_PushAround_Victim
{
	Priority PushAround_Victim
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Push", "2Hand", "Low"
		}
	}
}

//**********************************************************************
// PL_Manacles_Pose01
//**********************************************************************

PriorityList PL_Manacles
{
	Priority Manacles_Pose1
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Prop", "SLEEP", "WAIT", "LOW"
		}
	}
}

//**********************************************************************
// PL_Cross_Sword
//**********************************************************************

PriorityList PL_Cross_Sword
{
	Priority Cross_Sword
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"BLOCK", "Low", "High", "SWORD" ,"TELEKINESIS"      
		}
	}
}

//**********************************************************************
// PL_JackHammer_Vertical
//**********************************************************************

PriorityList PL_JackHammer_Vertical
{
	Priority JackHammer_Vertical
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Prop", "BUSY", "WORK", "LOW"
		}
	}
}

//**********************************************************************
// PL_JackHammer_Horizontal
//**********************************************************************

PriorityList PL_Jackhammer_Horizontal
{
	Priority Jackhammer_Horizontal
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Prop", "BUSY", "WORK", "HIGH"
		}
	}
}


//**********************************************************************
// PL_Camera
//**********************************************************************

PriorityList PL_Camera
{
	Priority Camera
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Shoot", "Point", "Cone", "Quick", "Happy"
		}
	}
}


//**********************************************************************
// PL_FlashlightUp
//**********************************************************************

PriorityList PL_FlashlightUp
{
	Priority Flashlight
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"OBSERVE", "BUSY", "HIGH"
		}
	}
}


//**********************************************************************
// PL_FlashlightDown
//**********************************************************************

PriorityList PL_FlashlightDown
{
	Priority Flashlight
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"OBSERVE", "BUSY", "LOW"
		}
	}
}


//**********************************************************************
// PL_Katana_Observe: Stand around looking 
//**********************************************************************

PriorityList PL_Katana_Observe
{
	Priority Katana_Observe
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "Katana", "Observe", "High"
		}
	}
}

//**********************************************************************
// PL_Katana_Lookout: 
//**********************************************************************

PriorityList PL_Katana_Lookout
{
	Priority Katana_Lookout
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Combat", "Weapon", "Katana", "Lookout", "High"
		}
	}
}


//**********************************************************************
// PL_Cross_Katana
//**********************************************************************

PriorityList PL_Cross_Katana
{
	Priority Cross_Katana
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"BLOCK", "Low", "High", "Katana" ,"TELEKINESIS"      
		}
	}
}

//**********************************************************************
// PL_Katana_Guard:
//**********************************************************************

PriorityList PL_Katana_Guard
{
	Priority Katana_Guard
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"combat", "Weapon", "katana", "lookout", "high", "low"
		}
	}
}


//**********************************************************************
// PL_Katana_Point:
//**********************************************************************

PriorityList PL_Katana_Point
{
	Priority Katana_Point
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"combat", "Weapon", "katana", "Point", "Strong"
		}
	}
}


//**********************************************************************
// PL_Katana_Alert:
//**********************************************************************

PriorityList PL_Katana_Alert
{
	Priority Katana_Alert
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Katana", "Pose", "Mid"
		}
	}
}


//**********************************************************************
// PL_Bum_Slump:
//**********************************************************************

PriorityList PL_Bum_Slump
{
	Priority Bum_Slump
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Sit", "LOVE", "Sad", "High", "Pose"
		}
	}
}


//**********************************************************************
// PL_Panhandle:
//**********************************************************************

PriorityList PL_Panhandle
{
	Priority Panhandle
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"cower", "axe", "sad"
		}
	}
}

//**********************************************************************
// PL_AssumeThePosition:
//**********************************************************************

PriorityList PL_AssumeThePosition
{
	Priority AssumeThePosition
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"CANTMOVE", "WAIT", "BUSY"
		}
	}
}

//**********************************************************************
// PL_Portable_Laptop:
//**********************************************************************

PriorityList PL_Portable_Laptop
{
	Priority Portable_Laptop
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"PROP", "BUSY", "CANTMOVE"
		}
	}
}



//**********************************************************************
// PL_Rumble_Sword_A: 
//**********************************************************************

PriorityList PL_Rumble_Sword_A
{
	Priority RumbleSword_A
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Sword", "RumbleA", "Low"
		}
	}
}


//**********************************************************************
// PL_Rumble_Katana_A: 
//**********************************************************************

PriorityList PL_Rumble_Katana_A
{
	Priority Rumble_Katana_A
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"katana", "RumbleA", "Low"
		}
	}
}


//**********************************************************************
// PL_Rumble_Katana_B: 
//**********************************************************************

PriorityList PL_Rumble_Katana_B
{
	Priority Rumble_Katana_B
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"katana", "RumbleB", "Low"
		}
	}
}


//**********************************************************************
// PL_PicketMarch: 
//**********************************************************************


PriorityList PL_PicketMarch
{
	Priority PicketMarch
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"Lookout", "Work", "Wait", "Pose", "Low"
		}

	}
}

//**********************************************************************
// PL_PeerIn
//**********************************************************************

PriorityList PL_PeerIn
{
	Priority PeerIn
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Observe", "Wall", "MID"
		}
	}
}

//**********************************************************************
// PL_Barf: 
//**********************************************************************

PriorityList PL_Barf
{
	Priority Barf
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Sad", "Blast", "Attack", "Encounter"
		}
	}
}


//**********************************************************************
// PL_Flip_Coin: 
//**********************************************************************

PriorityList PL_Flip_Coin
{
	Priority Flip_Coin
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RollDice", "Cast", "Low", "Encounter"
		}
	}
}



//**********************************************************************
// PL_Clipboard_Cross: 
//**********************************************************************

PriorityList PL_Clipboard_Cross
{
	Priority Clipboard_Cross
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Clipboard", "Talk" 
		}
	}
}


//**********************************************************************
// PL_Lotus_Float: 
//**********************************************************************

PriorityList PL_Lotus_Float
{
	Priority Lotus_Float
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"ENCOUNTERFLOATLOTUS"
		}
	}
}


//**********************************************************************
// PL_Newsie:
//**********************************************************************

PriorityList PL_Newsie
{
	Priority Newsie
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"ENCOUNTERNEWSIE"
		}
	}
}


//**********************************************************************
// PL_Golem_Ambush:
//**********************************************************************

PriorityList PL_Golem_Ambush
{
	Priority Golem_Ambush
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"ENCOUNTERGOLEMSPAWN"
		}
	}
}



//**********************************************************************
// PL_ClockworkSiphon:
//**********************************************************************

PriorityList PL_Clockwork_Siphon
{
	Priority Clockwork_Siphon
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"DRAIN", "SPELLCAST", "BUSY", "MID"
		}
	}
}

//**********************************************************************
// PL_ClockworkSiphon_Down:
//**********************************************************************

PriorityList PL_Clockwork_Siphon_Down
{
	Priority Clockwork_Siphon_Down
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"DRAIN", "SPELLCAST", "BUSY", "LOW"
		}
	}
}

//**********************************************************************
// PL_ClockworkSiphon_Up:
//**********************************************************************

PriorityList PL_Clockwork_Siphon_Up
{
	Priority Clockwork_Siphon_Up
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"DRAIN", "SPELLCAST", "BUSY", "HIGH"
		}
	}
}

//**********************************************************************
// PL_Pamphlet:
//**********************************************************************

PriorityList PL_Pamphlet
{
	Priority Pamphlet
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"PROP", "LOITER", "BECKON"
		}
	}
}

//**********************************************************************
// PL_PryBarBig:
//**********************************************************************

PriorityList PL_PryBarBig
{
	Priority PryBarBig
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"PROP", "WORK", "VANDAL"
		}
	}
}

//**********************************************************************
// PL_ThreeCardMonty:
//**********************************************************************

PriorityList PL_ThreeCardMonty
{
	Priority ThreeCardMonty
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"THREECARDMONTY" 
		}
	}
}

//**********************************************************************
// PL_PurseSnatchRun:
//**********************************************************************

PriorityList PL_PurseSnatchRun
{
	Priority RunAway
	{		
		Action: actionCombat
		Set:	me.feeling.fear = 20
		Set:	me.feeling.confidence = 10
		Set:	me.feeling.loyalty = 0
	}
}


//**********************************************************************
// PL_GuardNodOff:
//**********************************************************************

PriorityList PL_GuardNodOff
{
	Priority GuardNodOff
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"PROP", "SLEEP", "SIT" 
		}
	}
}

//**********************************************************************
// PL_PicketAngry
//**********************************************************************

PriorityList PL_PicketAngry
{
	Priority PicketAngry
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"ENCOUNTERPICKETANGRY" 
		}
	}
}


//**********************************************************************
// PL_Amused
//**********************************************************************

PriorityList PL_Amused
{
	Priority Amused
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Amused" 
		}
	}
}


//**********************************************************************
// PL_Farwell_Fadeout
//**********************************************************************

PriorityList PL_Farwell_Fadeout
{
	Priority Farwell_Fadeout
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"FarwellNfadeout" 
		}
	}
}



//**********************************************************************
// PL_Molotov_throw
//**********************************************************************

PriorityList PL_Molotov_throw
{
	Priority Molotov_throw
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"EncounterMolotov" 
		}
	}
}

//**********************************************************************
// PL_Snakeman_Ambush: 
//**********************************************************************

PriorityList PL_Snakeman_Ambush
{
	Priority Snakeman_Ambush
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spawn", "Low"
		}
	}
}

//**********************************************************************
// PL_Snake_Strike random combat for snakey
//**********************************************************************

PriorityList PL_Snake_Strike 
{
	Priority Snake_Strike
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RumbleA"
		}
	}
}

//**********************************************************************
// PL_Snake_Strike_Sword random combat for snakey
//**********************************************************************

PriorityList PL_Snake_Strike_Sword 
{
	Priority Snake_Strike
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"RumbleA", "Sword"
		}
	}
}

//**********************************************************************
// PL_Snake_Sunning sleeping snake spawn
//**********************************************************************

PriorityList PL_Snake_Sunning 
{
	Priority Snake_Sunning
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"sleep", "happy"
		}
	}
}

//**********************************************************************
// PL_Corpse_KIA:
//**********************************************************************

PriorityList PL_Corpse_KIA
{
	Priority Corpse_KIA
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"pose", "cower", "sad", "low"
		}
	}
}



//**********************************************************************
// PL_BodyBag: This just puts him in "bodybag" mode - for use in missions
//**********************************************************************

PriorityList PL_BodyBag
{
	Priority BodyBag
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"BODYBAG"
		}
	}
}



//**********************************************************************
// PL_BodyBag_Wander: A wander in "bodybag" mode
//**********************************************************************

PriorityList PL_BodyBag_Wander
{

	Priority BodyBag_Wander
	{		
		HoldPriority: true

		Action
		{
			Type:		actionWander
			Radius:		40
			TargetType:	targetSpawnPoint
			AnimBits:	"BODYBAG"
		}

	}
}



//**********************************************************************
// PL_BodyBag_Toss: Drops bodybag and just stands there - for mission use only
//**********************************************************************

PriorityList PL_BodyBag_Toss
{
	Priority BodyBag_Toss
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"BODYBAGTOSS"
		}
	}
}


//**********************************************************************
// PL_Bazul_Spawn: Bazul Demon rises out of lava
//**********************************************************************

PriorityList PL_Bazul_Spawn
{
	Priority Bazul_Spawn
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"BAZULSLEEP"
		}
	}
}


//**********************************************************************
// PL_Rocket_Launch: 
//**********************************************************************

PriorityList PL_Rocket_Launch
{
	Priority Rocket_Launch
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"LAUNCH"
		}
	}
}


//**********************************************************************
// PL_Rocket_Reload: 
//**********************************************************************

PriorityList PL_Rocket_Reload
{
	Priority Rocket_Reload
	{
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"RELOAD"
		}
	}
}


//**********************************************************************
// PL_Eyeball_Slumber:  
//**********************************************************************

PriorityList PL_Eyeball_Slumber
{
	Priority EyeballSlumber
	{
		Set:		me.untargetable = true
		Set:		me.flying = true
		Set:		me.findtarget = false
		Action
		{
			Type:		actionFrozenInPlace
			AnimBits:	"Spawn", "Low"
			
		}
	}
}


//**********************************************************************
// PL_Eyeball_Awake:  
//**********************************************************************

PriorityList PL_Eyeball_Awake
{
	Priority EyeballAwake
	{
		Set:		me.flying = true
		Set:		me.feeling.bothered = 100
		Action
		{
			Type:		actionCombat

		}
	}
}

//**********************************************************************
// PL_TouchScreen: Standing character Interracting with Touch screen
//**********************************************************************

PriorityList PL_TouchScreen
{
	Priority TouchScreen
	{
		Action {
			Type:		actionDoAnim
			AnimBits:	"TouchScreen"
		}
	}
}

//**********************************************************************
// PL_Unconscious (was PL_OutCold01)
//**********************************************************************

PriorityList PL_Unconscious
{
	Priority Unconscious
	{
		Action {
			Type:		actionDoAnim
			AnimBits:	"OUTCOLD"
		}
	}
}


//**********************************************************************
// PL_Parachute_Ambush:  
//**********************************************************************

PriorityList PL_Parachute_Ambush
{
	Priority Parachute_Ambush
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"PARACHUTEAMBUSH"
		}
	}
}


//**********************************************************************
// PL_Repel_Ambush:  
//**********************************************************************

PriorityList PL_Repel_Ambush
{
	Priority Repel_Ambush
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"REPELAMBUSH"
		}
	}
}


//**********************************************************************
// PL_Ninja_Ambush:  
//**********************************************************************

PriorityList PL_Ninja_Ambush
{
	Priority Ninja_Ambush
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"NINJAAMBUSH"
		}
	}
}

//**********************************************************************
// PL_Drinker:  
//**********************************************************************

PriorityList PL_Drinker
{
	Priority Drinker
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"DRINKER"
		}
	}
}

//**********************************************************************
// PL_Talk:  
//**********************************************************************

PriorityList PL_Talk
{
	Priority Talk
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"GESTICULATE"
		}
	}
}


//**********************************************************************
// PL_Fishing:  
//**********************************************************************

PriorityList PL_Fishing
{
	Priority Fishing
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"FISHING"
		}
	}
}

//**********************************************************************
// PL_FishingSeated:  
//**********************************************************************

PriorityList PL_Fishing_Low
{
	Priority Fishing_Low
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"FISHINGLOW"
		}
	}
}

//**********************************************************************
// PL_Purse_Mug:  
//**********************************************************************

PriorityList PL_Purse_Mug
{
	Priority Purse_Mug
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"PURSESEARCH"
		}
	}
}

//**********************************************************************
// PL_Bane_BombHole:  
//**********************************************************************

PriorityList PL_Bane_BombHole
{
	Priority PL_Bane_BombHole
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"BOMBHOLE"
		}
	}
}


//**********************************************************************
// PL_Bane_MaceScan:  
//**********************************************************************

PriorityList PL_Bane_MaceScan
{
	Priority PL_Bane_MaceScan
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"MACESCAN"
		}
	}
}

//**********************************************************************
// PL_Bane_MaceScanDown:  
//**********************************************************************

PriorityList PL_Bane_MaceScanDown
{
	Priority PL_Bane_MaceScanDown
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"MACESCANDOWN"
		}
	}
}

//**********************************************************************
// PL_Spider_Webdrop:  
//**********************************************************************

PriorityList PL_Spider_Webdrop
{
	Priority PL_Spider_Webdrop
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spawn", "High"
		}
	}
}

//**********************************************************************
// PL_Spider_WallAmbush:  
//**********************************************************************

PriorityList PL_Spider_WallAmbush
{
	Priority PL_Spider_WallAmbush
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"Spawn", "Wall"
		}
	}
}

//**********************************************************************
// PL_SpinCocoon:  
//**********************************************************************

PriorityList PL_SpinCocoon
{
	Priority PL_SpinCocoon
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"SPINCOCOON"
		}
	}
}

//**********************************************************************
// PL_Arachnoid_Sleep:  
//**********************************************************************

PriorityList PL_Arachnoid_Sleep
{
	Priority Arachnoid_Sleep
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"ARACHSLEEP"
		}
	}
}

//**********************************************************************
// PL_PurseTugPickup: Do pursesnatch animation.
//**********************************************************************

PriorityList PL_PurseTugPickup
{
	Priority PurseTugPickup
	{
		Action
		{
			Type:		actionDoAnim
			AnimBits:	"PURSEPICKUP"
		}
	}
}

//**********************************************************************
// PL_Typing: Standing character typing
//**********************************************************************

PriorityList PL_Typing
{
	Priority Typing
	{
		Action {
			Type:		actionDoAnim
			AnimBits:	"ENCOUNTERTYPING"
		}
	}
}

//**********************************************************************
// PL_Signature_Exit
//**********************************************************************

PriorityList PL_Signature_Exit
{
	Priority Signature_Exit
	{
		Action {
			Type:		actionDoAnim
			AnimBits:	"SIGNATUREEXIT"
		}
	}
}


//**********************************************************************
// PL_Cocoon_Sleep
//**********************************************************************

PriorityList PL_Cocoon_Sleep
{
	Priority Cocoon_Sleep
	{
		Action {
			Type:		actionDoAnim
			AnimBits:	"COCOONSLEEP"
		}
	}
}



