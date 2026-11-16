#############################################################
## PistolShellEject.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	Start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\PistolShellEject.bhvr
		Geom	PistolShell01
		Lifespan 1
	End
End

#############################################################

End