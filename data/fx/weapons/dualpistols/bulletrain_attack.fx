#############################################################
## Mercenary Mastermind Assault Rifle - Slug
#############################################################

FxInfo

LifeSpan 90

#############################################################

Condition
	On 	TriggerBits
	TriggerBits	MALE

	Event
		EName	MotionBlur_Male
		Type	Local
		At	Root
		ChildFX	:Child_BulletRain_MotionBlur_Male.fx
	End
End

Condition
	On 		TriggerBits
	TriggerBits	FEMALE

	Event
		EName	MotionBlur_Fem
		Type	Local
		At	Root
		ChildFX	:Child_BulletRain_MotionBlur_Fem.fx
	End
End

Condition
	On 	TriggerBits
	TriggerBits	HUGE

	Event
		EName	MotionBlur_Huge
		Type	Local
		At	Root
		ChildFX	:Child_BulletRain_MotionBlur_Huge.fx
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
	Time 	13

	Event
		Type	Local
		At	MuzzleFlashOffset_Left
		ChildFX	WEAPONS\DualPistols\Child_CycleShot_Rapid.fx
		Lifespan 4
	End
End

Condition
	On 	Time
	Time 	23

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot_Rapid.fx
		Lifespan 3
	End
End

Condition
	On 	Time
	Time 	32

	Event
		Type	Local
		At	MuzzleFlashOffset_Left
		ChildFX	WEAPONS\DualPistols\Child_CycleShot_Rapid.fx
		Lifespan 3
	End
End

Condition
	On 	Time
	Time 	40

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot_Rapid.fx
		Lifespan 4
	End
End
#
#Condition
#	On 	Time
#	Time 	50
#
#	Event
#		Type	Local
#		At	MuzzleFlashOffset_Left
#		ChildFX	WEAPONS\DualPistols\Child_CycleShot_Rapid.fx
#		Lifespan 4
#	End
#End

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
	Time 	5

	Event
		Type	Local
		At	BendMystic
		ChildFX	WEAPONS\DualPistols\Child_BulletRain_Right.fx
		Lifespan 20
	End
End

Condition
	On 	Time
	Time 	14

	Event
		Type	Local
		At	BendMystic
		ChildFX	WEAPONS\DualPistols\Child_BulletRain_Left.fx
		Lifespan 20
	End
End

Condition
	On 	Time
	Time 	24

	Event
		Type	Local
		At	BendMystic
		ChildFX	WEAPONS\DualPistols\Child_BulletRain_Right.fx
		Lifespan 20
	End
End

Condition
	On 	Time
	Time 	33

	Event
		Type	Local
		At	BendMystic
		ChildFX	WEAPONS\DualPistols\Child_BulletRain_Left.fx
		Lifespan 20
	End
End

Condition
	On 	Time
	Time 	41

	Event
		Type	Local
		At	BendMystic
		ChildFX	WEAPONS\DualPistols\Child_BulletRain_Right.fx
		Lifespan 20
	End
End

#Condition
#	On 	Time
#	Time 	51
#
#	Event
#		Type	Local
#		At	BendMystic
#		ChildFX	WEAPONS\DualPistols\Child_BulletRain_Left.fx
#		Lifespan 20
#	End
#End

#############################################################

End