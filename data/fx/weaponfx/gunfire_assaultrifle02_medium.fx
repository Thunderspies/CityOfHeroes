#############################################################
## Gunfire_AssaultRifle02_Medium.fx
#############################################################

FxInfo

LifeSpan 60

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		ENAME   GunOffset
		Type	Local
		At	WepR
		BHVR	Behaviors\MachineGunBustMedium2.bhvr
		Geom	FlameThrower
		Sound Assault_long 120.0 120.0 .9
	End

	Event
		Type	local
		At	GunOffset
		Altpiv	1
		Part1	WEAPONFX\TorchflameA.part
		Part2	WEAPONFX\TorchflameB.part
		LifeSpan 44
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		Type	local
		At	GunOffset
		Altpiv	1
		ChildFX	WEAPONFX\Child_MuzzleFlash1.fx
		Lifespan 44
	End

	Event
		Type	Local
		At	WepR
		ChildFX	WEAPONFX\Child_AssaultRifleShellEject.fx
		Lifespan 44
	End
End

#############################################################

End