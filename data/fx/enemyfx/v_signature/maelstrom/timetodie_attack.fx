#############################################################
## Mercenary Mastermind Assault Rifle - Slug
#############################################################

FxInfo
LifeSpan 90

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	Root
		Bhvr	ENEMYFX\V_Signature\Maelstrom\MBlur_BulletRain_A1.bhvr
		BhvrOverride
			Alpha			128
		End
		Anim	V_Maelstrom_StageTwo
		Lifespan 65
	End
End

Condition
	On 	Time
	Time 	9

	Event
		Type	Local
		At	Root
		Bhvr	ENEMYFX\V_Signature\Maelstrom\MBlur_BulletRain_A1.bhvr
		BhvrOverride
			Alpha			96
		End
		Anim	V_Maelstrom_StageTwo
		Lifespan 64
	End
End

Condition
	On 	Time
	Time 	12

	Event
		Type	Local
		At	Root
		Bhvr	ENEMYFX\V_Signature\Maelstrom\MBlur_BulletRain_A1.bhvr
		BhvrOverride
			Alpha			64
		End
		Anim	V_Maelstrom_StageTwo
		Lifespan 63
	End
End

#############################################################

Condition
	On	Time
	Time	28

	Event
		Type	Local
		At	Root
		sound	DW_BulletRain_01 120.0 100.0 1.0
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	MuzzleFlashOffset_Right
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset		0.75 0 0.375
		End
	End
End

## MUZZLEFLASHES ###########################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot_Rapid.fx
		Lifespan 4
	End
End

Condition
	On 	Time
	Time 	39

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot_Rapid.fx
		Lifespan 4
	End
End

Condition
	On 	Time
	Time 	54

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot_Rapid.fx
		Lifespan 3
	End
End

Condition
	On 	Time
	Time 	53

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot_Rapid.fx
		Lifespan 3
	End
End

Condition
	On 	Time
	Time 	61

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot_Rapid.fx
		Lifespan 4
	End
End

Condition
	On 	Time
	Time 	75

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot_Rapid.fx
		Lifespan 4
	End
End

## CURVING TRAILS ###########################################################

Condition
	On 	Time
	Time 	26

	Event
		Type	Local
		At	BendMystic
		ChildFX	WEAPONS\DualPistols\Child_BulletRain_Right.fx
		Lifespan 20
	End
End

Condition
	On 	Time
	Time 	40

	Event
		Type	Local
		At	BendMystic
		ChildFX	WEAPONS\DualPistols\Child_BulletRain_Right.fx
		Lifespan 20
	End
End

Condition
	On 	Time
	Time 	45

	Event
		Type	Local
		At	BendMystic
		ChildFX	WEAPONS\DualPistols\Child_BulletRain_Right.fx
		Lifespan 20
	End
End

Condition
	On 	Time
	Time 	54

	Event
		Type	Local
		At	BendMystic
		ChildFX	WEAPONS\DualPistols\Child_BulletRain_Right.fx
		Lifespan 20
	End
End

Condition
	On 	Time
	Time 	62

	Event
		Type	Local
		At	BendMystic
		ChildFX	WEAPONS\DualPistols\Child_BulletRain_Right.fx
		Lifespan 20
	End
End

Condition
	On 	Time
	Time 	76

	Event
		Type	Local
		At	BendMystic
		ChildFX	WEAPONS\DualPistols\Child_BulletRain_Right.fx
		Lifespan 20
	End
End

#############################################################

End