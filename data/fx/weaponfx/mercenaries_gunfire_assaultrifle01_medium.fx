#############################################################
## Mercenaries_Gunfire_AssaultRifle01_Medium.fx
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
		Geom	Mastermind_Military_Rifle
		Sound SCARHeavyBurst1 120.0 120.0 .6
	End

	Event
		Type	local
		At	GunOffset
		Altpiv	2
		Bhvr	Behaviors\MachineGunBust2.bhvr
		Part1	WEAPONFX\TorchflameBlue1.part
		Part2	WEAPONFX\TorchflameBlue2.part
		LifeSpan 47
	End
End

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		Type	local
		At	GunOffset
		Altpiv	2
		ChildFX	WEAPONFX\Child_SilencedMuzzleFlash1.fx
		Lifespan 47
	End

	Event
		Type	Local
		At	WepR
		ChildFX	WEAPONFX\Child_AssaultRifleShellEject.fx
		Lifespan 47
	End
End

#############################################################

End