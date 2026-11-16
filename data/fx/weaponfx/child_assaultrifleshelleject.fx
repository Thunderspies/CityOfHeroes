#############################################################
## Child Effect - Muzzle Flash Cycle (Rapid Fire)
#############################################################

FxInfo

Lifespan 90

#############################################################

Condition
	On 	Cycle
	Time 	3

	Event
		Type	Local
		At	Origin
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
		Lifespan 1
	End
End

#############################################################

End