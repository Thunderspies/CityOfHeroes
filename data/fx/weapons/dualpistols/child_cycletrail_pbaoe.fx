#############################################################
## Mercenary Mastermind Assault Rifle - Slug
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	PositOnly
		At	Chest
		BhvrOverride
			PyrRotate		0 0 -15
			PyrRotateJitter		0 180 30
		End
		ChildFX	WEAPONS\DualPistols\Child_VaporTrail_PBAOE.fx
	End
End

Condition
	On 	Cycle
	Time 	2

	Event
		EName	Cycle
		Type	PositOnly
		At	Chest
		BhvrOverride
			PyrRotate		0 0 -15
			PyrRotateJitter		0 180 30
		End
		ChildFX	WEAPONS\DualPistols\Child_VaporTrail_PBAOE.fx
		LifeSpan 10
	End
End

Condition
	On 	Cycle
	Time 	2

	Event
		EName	Cycle
		Type	PositOnly
		At	Chest
		BhvrOverride
			PyrRotate		0 0 -15
			PyrRotateJitter		0 180 30
		End
		ChildFX	WEAPONS\DualPistols\Child_VaporTrail_PBAOE_TracersOnly.fx
		LifeSpan 10
	End
End

#############################################################

End