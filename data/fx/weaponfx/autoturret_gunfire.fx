#############################################################
## AutoTurret_Gunfire.fx
#############################################################

FxInfo
Flags InheritAlpha
LifeSpan 60

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	lazer
		Type	local
		At	Hips
		Sound gatgun3 80 80 .8
		Geom	turrentRotate
	End

	Event
		EName	light
		Type	local
		At	lazer
		Altpiv	1
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	tracer
		Type	local
		At	lazer
		Altpiv	1
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
	End

	Event
		EName	light
		Type	local
		At	lazer
		Altpiv	2
		Geom	FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	tracer
		Type	local
		At	lazer
		Altpiv	2
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
	End

	Event
		EName	light
		Type	local
		At	lazer
		Altpiv	3
		Geom	leftBarrell
		Bhvr	Behaviors/BarrellSpin2.bhvr
	End

	Event
		EName	light
		Type	local
		At	lazer
		Altpiv	4
		Geom	rightBarrell
		Bhvr	Behaviors/BarrellSpin2.bhvr
	End
End

#############################################################

End