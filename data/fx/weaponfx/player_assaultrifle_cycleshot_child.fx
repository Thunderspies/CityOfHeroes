#############################################################
## Assault Rifle - Child: Single Shot
#############################################################

FxInfo

#############################################################

Condition
	On 	Cycle
	Time 	3

	Event
		Type	Local
		At	C_Chest
		Bhvr	Behaviors/MachineGunBust.bhvr
		Geom	FX_MachineGunBurst
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
		Lifespan	2
	End

	Event
		Type	Local
		At	C_Head
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
		Lifespan	1
	End
End

#############################################################

End