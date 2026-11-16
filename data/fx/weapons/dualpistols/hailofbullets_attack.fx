#############################################################
## HailOfBullets_Attack.fx
#############################################################

FxInfo

LifeSpan 120

#############################################################

Condition
	On 	TriggerBits
	TriggerBits	MALE

	Event
		EName	MotionBlur_Male
		Type	Local
		At	Root
		ChildFX	:Child_HailOfBullets_MotionBlur_Male.fx
	End
End

Condition
	On 		TriggerBits
	TriggerBits	FEMALE

	Event
		EName	MotionBlur_Fem
		Type	Local
		At	Root
		ChildFX	:Child_HailOfBullets_MotionBlur_Fem.fx
	End
End

Condition
	On 	TriggerBits
	TriggerBits	HUGE

	Event
		EName	MotionBlur_Huge
		Type	Local
		At	Root
		ChildFX	:Child_HailOfBullets_MotionBlur_Huge.fx
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		sound	DW_HailOfBullets_01 120.0 100.0 1.0
	End

	Event
		EName	MuzzleFlashOffset_Left
		Type	Local
		At	WepL
		BhvrOverride
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

#############################################################

Condition
	On 	Time
	Time 	6

	Event
		Type	Local
		At	MuzzleFlashOffset_Left
		BhvrOverride
			PyrRotate		0 180 0
		End
		ChildFX	WEAPONS\DualPistols\Child_CycleShot.fx
		Lifespan 104
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot.fx
		Lifespan 102
	End
End

Condition
	On 	Time
	Time 	6

	Event
		Type	PositOnly
		At	Chest
		ChildFX	WEAPONS\DualPistols\Child_CycleTrail_PBAOE.fx
		Lifespan 104
	End
End


#############################################################

#Condition
#	On 	Time
#	Time 	55
#
#	Event
#		Type	Local
#		At	MuzzleFlashOffset_Left
#		BhvrOverride
#			PyrRotate		0 180 0
#		End
#		ChildFX	WEAPONS\DualPistols\Child_CycleShot.fx
#		Lifespan 1
#	End
#
#	Event
#		Type	Local
#		At	MuzzleFlashOffset_Left
#		ChildFX	WEAPONS\DualPistols\Child_VaporTrail.fx
#		Lifespan 300
#	End
#
#	Event
#		Type	Local
#		At	MuzzleFlashOffset_Right
#		ChildFX	WEAPONS\DualPistols\Child_CycleShot.fx
#		Lifespan 1
#	End
#
#	Event
#		Type	Local
#		At	MuzzleFlashOffset_Right
#		ChildFX	WEAPONS\DualPistols\Child_VaporTrail.fx
#		Lifespan 300
#	End
#End

#############################################################

#Condition
#	On 	Time
#	Time 	14
#
#	Event
#		Type	Local
#		At	BendMystic
#		ChildFX	WEAPONS\DualPistols\Child_CycleTrail_Cone.fx
#		Lifespan 38
#	End
#End

#############################################################

End