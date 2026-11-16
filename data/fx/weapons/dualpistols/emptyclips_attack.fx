#############################################################
## EmptyClips_Attack.fx
#############################################################

FxInfo

Flags InheritAnimScale

LifeSpan 90

#############################################################

Condition
	On 	Time
	Time 	0

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
	Time 	3

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
	Time 	7

	Event
		Type	Local
		At	BendMystic 
		BhvrOverride
				PyrRotate			-20 80 0
				PositionOffset 1 1 -1.5
		End
		ChildFX	WEAPONS\DualPistols\Child_CycleTrail_Cone.fx
		Lifespan 10
	End
End

Condition
	On		Time
	Time	23
Event
		Type	Start
		At	BendMystic 
		BhvrOverride
				PyrRotate			-20 10 0
				PositionOffset 0.5 0 -1
		End
		ChildFX	WEAPONS\DualPistols\Child_CycleTrail_Cone.fx
		Lifespan 7
	End
End

Condition
	On		Time
	Time	19
Event
		Type	Local
		At	BendMystic 
		BhvrOverride
				PyrRotate			-10 30 0
				PositionOffset 1.5 0 -1
		End
		ChildFX	WEAPONS\DualPistols\Child_CycleTrail_Cone.fx
		Lifespan 23
	End
End


#############################################################

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot.fx 
		
		Lifespan 12 
	End  
	
End

Condition
	On 	Time
	Time 	22

	Event
		Type	Local
		At	MuzzleFlashOffset_Right
		ChildFX	WEAPONS\DualPistols\Child_CycleShot.fx
		Lifespan 12
	End 
	
End

Condition
	On 	Time
	Time 	34

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
	Time 	9.5

	Event
		Type	Local
		At	MuzzleFlashOffset_Left
		BhvrOverride
			PyrRotate		0 180 0
		End
		ChildFX	WEAPONS\DualPistols\Child_CycleShot.fx
		Lifespan 12
	End
	 
End

Condition
	On 	Time
	Time 	26

	Event
		Type	StartPositOnly
		At	MuzzleFlashOffset_Left
		BhvrOverride
			PyrRotate		0 100 0
		End
		ChildFX	WEAPONS\DualPistols\Child_CycleShot.fx
		Lifespan 12
	End 
	
End

Condition
	On 	Time
	Time 	38.5

	Event
		Type	Local
		At	MuzzleFlashOffset_Left
		BhvrOverride
			PyrRotate		0 180 0
		End
		ChildFX	WEAPONS\DualPistols\Child_CycleShot.fx
		Lifespan 8
	End 
	
End

#############################################################

End