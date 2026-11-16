#############################################################
## ARGunShellEject.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\ARGunShellEject.bhvr
		Geom	ArtilleryShell
		Lifespan 90
		LifespanJitter 30
	End
End

#############################################################

End