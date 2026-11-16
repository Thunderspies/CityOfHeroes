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
		Geom	Tintable_FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	tracer
		Type	local
		At	lazer
		Altpiv	1
		Part1	:TorchflameA.part
		Part2	:TorchflameB.part
	End

	Event
		EName	light
		Type	local
		At	lazer
		Altpiv	2
		Geom	Tintable_FX_MachineGunBurst
		Bhvr	Behaviors/MachineGunBust.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part1	:MachineGunFlash.part
		Part2	:MachineGunGlow.part
	End

	Event
		EName 	tracer
		Type	local
		At	lazer
		Altpiv	2
		Part1	:TorchflameA.part
		Part2	:TorchflameB.part
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