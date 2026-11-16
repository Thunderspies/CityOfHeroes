#############################################################
## Gunfire_AssaultRifle01_Long.fx
#############################################################

FxInfo

LifeSpan 180

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		ENAME   GunOffset
		Type	Local
		At	WepR
		Geom	FlameThrower
		Sound FullAuto 120.0 120.0 0.9
	End

	Event
		Type	local
		At	GunOffset
		Altpiv	1
		Part1	WEAPONFX\TorchflameA.part
		Part2	WEAPONFX\TorchflameB.part

	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		Type	Local
		At	GunOffset
		Altpiv	1
		ChildFX	WEAPONFX\Child_MuzzleFlash1.fx
		Lifespan 112
	End

	Event
		Type	Local
		At	WepR
		ChildFX	WEAPONFX\Child_AssaultRifleShellEject.fx
		Lifespan 112
	End
End

#############################################################

End