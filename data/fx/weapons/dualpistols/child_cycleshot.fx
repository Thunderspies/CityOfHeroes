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
		ChildFX	WEAPONS\DualPistols\Child_SingleShot.fx
		Lifespan 2
	End

	Event
		Type	Start
		At	Origin
		ChildFX	WEAPONS\DualPistols\Child_ShellEject.fx
		Lifespan 1
	End
End

Condition
	On 	Cycle
	Time 	4

	Event
		Type	Start
		At	Origin
		ChildFX	WEAPONS\DualPistols\Child_SingleShot.fx
		Lifespan 2
	End

	Event
		Type	Start
		At	Origin
		ChildFX	WEAPONS\DualPistols\Child_ShellEject.fx
		Lifespan 1
	End
End

#############################################################

End