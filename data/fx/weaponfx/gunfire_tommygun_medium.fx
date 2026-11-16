#############################################################
## Gunfire_TommyGun_Medium.fx
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
		Geom	TommyGunWepR
		Sound tommygunBurst 120.0 120.0 1.0
	End

	Event
		Type	Local
		At	GunOffset
		Altpiv	1
		Part1	WEAPONFX\TorchflameA.part
		Part2	WEAPONFX\TorchflameB.part
		LifeSpan 41
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
		Lifespan 41
	End

	Event
		Type	Local
		At	WepR
		ChildFX	WEAPONFX\Child_AssaultRifleShellEject.fx
		Lifespan 41
	End
End

#############################################################

End