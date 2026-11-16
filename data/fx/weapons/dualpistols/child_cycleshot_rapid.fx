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
		Type 	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\ARGunShellEject.bhvr
		Geom	PistolShell01
		Lifespan 90
		LifespanJitter 30
	End
End

Condition
	On 	Cycle
	Time 	1

	Event
		Type	Start
		At	Origin
		ChildFX	WEAPONS\DualPistols\Child_SingleShot.fx
		Lifespan 2
	End

	Event
		Type 	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\ARGunShellEject.bhvr
		Geom	PistolShell01
		Lifespan 90
		LifespanJitter 30
	End
End

#############################################################

End