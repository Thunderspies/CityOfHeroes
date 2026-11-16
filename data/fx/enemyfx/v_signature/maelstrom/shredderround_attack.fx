#############################################################
## EmptyClips_Attack.fx
#############################################################

FxInfo

Flags InheritAnimScale

LifeSpan 90

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		EName	MuzzleFlashOffset_Left
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset		-0.75 0 0.375
		End
		Lifespan 55
	End

	Event
		EName	MuzzleFlashOffset_Right
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset		0.75 0 0.375
		End
		Lifespan 55
	End
End

Condition
	On 	Time
	Time 	35

	Event
		Type	Local
		At	Root
		sound	DW_EmptyClips_01 120.0 100.0 1.0
		Lifespan 1
	End
End

#############################################################

Condition
	On 	Time
	Time 	44

	Event
		Type	Local
		At	WepR
		ChildFX	WEAPONS\DualPistols\Child_CycleTrail_Cone.fx
		Lifespan 40
	End
End

#############################################################

Condition
	On 	Time
	Time 	43

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot.fx
		Lifespan 12
	End
End

Condition
	On 	Time
	Time 	57

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot.fx
		Lifespan 12
	End
End

Condition
	On 	Time
	Time 	80

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot.fx
		Lifespan 12
	End
End

#############################################################

Condition
	On 	Time
	Time 	44.5

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot.fx
		Lifespan 12
	End
End

Condition
	On 	Time
	Time 	71

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot.fx
		Lifespan 12
	End
End

Condition
	On 	Time
	Time 	73.5

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot.fx
		Lifespan 8
	End
End

Condition
	On 	Time
	Time 	85.5

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot.fx
		Lifespan 8
	End
End

#############################################################

End