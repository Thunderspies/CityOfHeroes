#############################################################
## Gunfire_ChainGun_Medium.fx
#############################################################

FxInfo

LifeSpan 60

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		EName	GunOffset
		Type	Local
		At	WepR
		Bhvr	Behaviors\ChainGunBustMedium.bhvr
		Geom	FlameThrower
		Sound chaingun_med2 120.0 120.0 1.0
	End

	Event
		Type	Local
		At	GunOffset
		Altpiv	1
		Bhvr	Behaviors\BarrellSpin3.bhvr
		Geom	FX_ChainGunBarrell
		lifespan 44
	End

	Event
		Type	Local
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