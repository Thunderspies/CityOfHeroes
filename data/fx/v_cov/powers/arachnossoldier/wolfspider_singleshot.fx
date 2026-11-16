#############################################################
## Arachnos Soldier - Wolf Spider - Single Shot
#############################################################

FxInfo

Lifespan 45

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		Type	Local
		At	C_Chest
		Bhvr	Behaviors\MachineGunBust.bhvr
		Geom	FX_MachineGunBurst
		Part1	WEAPONFX\MachineGunFlash.part
		Part2	WEAPONFX\MachineGunGlow.part
		Part2	WEAPONFX\Gunfire_pistolsmoke.part
		Sound heavypistol1 100.0 100.0 1.0
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