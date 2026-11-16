#############################################################
## Assault Rifle - Sniper Rifle Attack FX
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	fire
		Type	local
		At	C_Chest
		Part1	WEAPONFX\TorchflameAb.part
		Part2	WEAPONFX\TorchflameBa.part
		Part3	WEAPONFX\Gunfire_ShotgunMuzzflashSmall.part
		Geom	FX_MachineGunBurst
		Lifespan 1
	End

	Event
		EName 	SmokeBurst
		Type	local
		At	C_Chest
		Part2	WEAPONFX\Gunfire_Shotgunsmoke.part
		Lifespan 10
	End

	Event
		ENAME   shell
		Type	Local
		At	C_Head
		ChildFX	V_COV\PhysicsEnabled\ARGunShellEject.fx
		Sound sniper1 120.0 120.0 1.0
	End
End

#############################################################

End