#############################################################
## Assault Rifle - Burst Attack FX
#############################################################

FxInfo

LifeSpan 30

#############################################################

Condition
	On 	Time
	Time 	7

	Event
		ENAME   p2
		Type	Local
		At	WepR
		Geom	Mastermind_Military_Rifle
		Sound SCARBurst1 120.0 120.0 .6
		Lifespan 14
	End

	Event
		EName 	tracer
		Type	local
		At	p2
		Altpiv	2
		BHVR	WEAPONs\WeaponRotate.bhvr
		Part1	WEAPONFX/TorchflameBlue1.part
		Part2	WEAPONFX/TorchflameBlue2.part
		Lifespan 12
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv	2
		BHVR	WEAPONs\MussleFlashOffset2.bhvr
		Part1	:SilencedMachineGunFlash.part
		Part2	:SilencedMachineGunGlow.part
		Part2	:SilencedMachineGunFlashcore.part
		Lifespan 3
	End
End

Condition
	On 	Time
	Time 	8

	Event
		EName 	Blast2
		Type	local
		At	p2
		Altpiv	2
		BHVR	WEAPONs\MussleFlashOffset2.bhvr
		Part1	:SilencedMachineGunFlash.part
		Part2	:SilencedMachineGunGlow.part
		Part2	:SilencedMachineGunFlashcore.part
		Lifespan 3
	End
End

Condition
	On 	Time
	Time 	11

	Event
		EName 	Blast3
		Type	local
		At	p2
		Altpiv	2
		BHVR	WEAPONs\MussleFlashOffset2.bhvr
		Part1	:SilencedMachineGunFlash.part
		Part2	:SilencedMachineGunGlow.part
		Part2	:SilencedMachineGunFlashcore.part
		Lifespan 3
	End
End

Condition
	On 	Time
	Time 	14

	Event
		EName 	Blast4
		Type	local
		At	p2
		Altpiv	2
		BHVR	WEAPONs\MussleFlashOffset2.bhvr
		Part1	:SilencedMachineGunFlash.part
		Part2	:SilencedMachineGunGlow.part
		Part2	:SilencedMachineGunFlashcore.part
		Lifespan 3
	End
End

Condition
	On 	Time
	Time 	17

	Event
		EName 	Blast5
		Type	local
		At	p2
		Altpiv	2
		BHVR	WEAPONs\MussleFlashOffset2.bhvr
		Part1	:SilencedMachineGunFlash.part
		Part2	:SilencedMachineGunGlow.part
		Part2	:SilencedMachineGunFlashcore.part
		Lifespan 3
	End
End

#############################################################

End