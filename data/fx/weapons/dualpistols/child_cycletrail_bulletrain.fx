#############################################################
## Mercenary Mastermind Assault Rifle - Slug
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
		ChildFX	WEAPONS\DualPistols\Child_VaporTrail_BulletRain.fx
		LifeSpan 10
	End
End

Condition
	On 	Cycle
	Time 	9

	Event
		EName	Cycle
		Type	Start
		At	Origin
		ChildFX	WEAPONS\DualPistols\Child_VaporTrail_BulletRain.fx
		LifeSpan 60
	End
End

#############################################################

End