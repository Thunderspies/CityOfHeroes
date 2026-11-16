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
		Anim	FX_Maelstrom_MBlur
		Lifespan 35
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
		Anim	FX_Maelstrom_MBlur
		Lifespan 34
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
		Anim	FX_Maelstrom_MBlur
		Lifespan 33
	End
End

#############################################################

Condition
	On	Time
	Time	8

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
		EName	MuzzleFlashOffset_Left
		Type	Local
		At	WepL
		BhvrOverride
			PyrRotate		0 180 0
			PositionOffset		-0.75 0 0.375
		End
	End

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
	Time 	5

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot_Rapid.fx
		Lifespan 4
	End
End

Condition
	On 	Time
	Time 	14

	Event
		Type	Local
		At	MuzzleFlashOffset_Left
		ChildFX	WEAPONS\DualPistols\Child_CycleShot_Rapid.fx
		Lifespan 4
	End
End

Condition
	On 	Time
	Time 	24

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot_Rapid.fx
		Lifespan 3
	End
End

Condition
	On 	Time
	Time 	33

	Event
		Type	Local
		At	MuzzleFlashOffset_Left
		ChildFX	WEAPONS\DualPistols\Child_CycleShot_Rapid.fx
		Lifespan 3
	End
End

Condition
	On 	Time
	Time 	41

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot_Rapid.fx
		Lifespan 4
	End
End

Condition
	On 	Time
	Time 	50

	Event
		Type	Local
		At	MuzzleFlashOffset_Left
		ChildFX	WEAPONS\DualPistols\Child_CycleShot_Rapid.fx
		Lifespan 4
	End
End

#### INTO AIR TRAILS ###########################################################
##
##Condition
##	On 	Time
##	Time 	6
##
##	Event
##		Type	Local
##		At	Root
##		ChildFX	WEAPONS\DualPistols\Child_CycleTrail_BulletRain.fx
##		Lifespan 53
##	End
##End

## CURVING TRAILS ###########################################################

Condition
	On 	Time
	Time 	6

	Event
		Type	Local
		At	BendMystic
		ChildFX	WEAPONS\DualPistols\Child_BulletRain_Right.fx
		Lifespan 20
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	BendMystic
		ChildFX	WEAPONS\DualPistols\Child_BulletRain_Left.fx
		Lifespan 20
	End
End

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	BendMystic
		ChildFX	WEAPONS\DualPistols\Child_BulletRain_Right.fx
		Lifespan 20
	End
End

Condition
	On 	Time
	Time 	34

	Event
		Type	Local
		At	BendMystic
		ChildFX	WEAPONS\DualPistols\Child_BulletRain_Left.fx
		Lifespan 20
	End
End

Condition
	On 	Time
	Time 	42

	Event
		Type	Local
		At	BendMystic
		ChildFX	WEAPONS\DualPistols\Child_BulletRain_Right.fx
		Lifespan 20
	End
End

Condition
	On 	Time
	Time 	51

	Event
		Type	Local
		At	BendMystic
		ChildFX	WEAPONS\DualPistols\Child_BulletRain_Left.fx
		Lifespan 20
	End
End

#############################################################

End