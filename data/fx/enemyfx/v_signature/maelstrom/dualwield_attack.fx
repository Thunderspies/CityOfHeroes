#############################################################
## DualWield_Attack.fx
#############################################################

FxInfo

Flags InheritAnimScale

LifeSpan 30

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Root
		sound	DW_DualWield_01 120.0 100.0 1.0
	End
End

#############################################################

Condition
	On 	Time
	Time 	19

	Event
		EName	MuzzleFlashOffset_Left
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset		0.75 0 0.375
		End
		ChildFX	WEAPONS\DualPistols\Child_VaporTrail.fx
	End

	Event
		Type	Start
		At	MuzzleFlashOffset_Left
		ChildFX	WEAPONS\DualPistols\Child_SingleShot.fx
	End

	Event
		Type	Start
		At	WepR
		BhvrOverride
			PositionOffset		0 0.25 0
		End
		ChildFX	WEAPONS\DualPistols\Child_ShellEject.fx
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName	MuzzleFlashOffset_Right
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset		-0.75 0 0.375
		End
		ChildFX	WEAPONS\DualPistols\Child_VaporTrail.fx
	End

	Event
		Type	Start
		At	MuzzleFlashOffset_Right
		BhvrOverride
			PyrRotate		0 180 0
		End
		ChildFX	WEAPONS\DualPistols\Child_SingleShot.fx
	End

	Event
		Type	Start
		At	WepL
		BhvrOverride
			PositionOffset		0 0.5 0
		End
		ChildFX	WEAPONS\DualPistols\Child_ShellEject.fx
		Lifespan 1
	End
End

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		Ename	Prime
		Type	Start
		At	Origin
		BhvrOverride
			TrackRate		5.0
		End
		LookAt	Target
		Magnet	Target
	End
End

Condition
	On	PrimeHit

	Event
		Type	Destroy
		Ename	ALL
	End
End

#############################################################

End