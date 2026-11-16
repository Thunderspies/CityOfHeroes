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
		At	C_Chest
		Bhvr	Behaviors\MachineGunBust.bhvr
		Part1	WEAPONFX\MachineGunFlash.part
		Part2	WEAPONFX\MachineGunGlow.part
		Geom	FX_MachineGunBurst
		Lifespan 2
	End

	Event
		Type	Local
		At	C_Head
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
		Lifespan 1
	End
End

#############################################################

End