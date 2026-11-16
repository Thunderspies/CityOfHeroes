#############################################################
## Drone.fx
#############################################################

FxInfo

Flags    InheritAlpha

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Root
		Sound CompBeep6_loop 120 120 0.46
		Lifespan 10
	End

	Event
		Type	Start
		At	Root
		Sound M1.ogg 120 120 0.46
		Lifespan 10
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Root
		Sound targetdrone_loop 30 30 0.3
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 120
	End

	Event
		EName	DroneAttachment1
		Type	local
		At	Root
		Bhvr	Behaviors\DroneSpin.bhvr
		geom	OverHeadDummy
	End

	Event
		EName	DroneAttachment
		Type	local
		At	DroneAttachment1
		altpiv	2
		Bhvr	Behaviors\DroneSpinLocal.bhvr
		geom	centerdummy
	End

	Event
		EName	Drone
		Type	local
		At	DroneAttachment
		Bhvr	Behaviors\DroneFade.bhvr
		Geom	FX_Drone
		Part	:glow.part
	End
End

Condition
	On 	Time
	Time 	2

	Event
		Type	local
		At	drone
		Part	Powers\ElectricityControl\ElectricityArchDroneSmall2.part
		Part	Powers\ElectricityControl\ElectricityArchContinuing.part
		Part	Powers\ElectricityControl\ElectricityArchContinuing2.part
		LifeSpan 50
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Drone
		AltPiv	1
		Part1	:DroneLazerLight2.part
		Part	:GunLightPulsing1.part
	End

	Event
		Type	local
		At	drone
		AltPiv	2
		Part1	:DroneLazerLight2.part
		Part	:GunLightPulsing1.part
	End

	Event
		Type	local
		At	drone
		AltPiv	3
		Part1	:DroneLazerLight2.part
		Part	:GunLightPulsing1.part
	End

	Event
		EName	light
		Type	local
		At	drone
		AltPiv	4
		Part1	:DroneLazerLight2.part
		Part	:GunLightPulsing1.part
	End
End

#############################################################

Condition
	On		TriggerBits
	TriggerBits	DEATH

	Event
		Type	local
		At	drone
		Part1	Powers\ElectricityControl\ElectricityGlow.part
		Part2	Powers\ElectricityControl\ElectricityGlowStar.part
		Part3	Powers\ElectricityControl\ElectricityArch4a.part
		Part4	Powers\ElectricityControl\ElectricityArchSmalla.part
		Sound elecballhit 80 80 0.8
		LifeSpan 30
	End

	Event
		Type	local
		At	drone
		Part1	Powers\ElectricityControl\ElectricityArchContinuing.part
		Part2	Powers\ElectricityControl\ElectricityArchContinuing2.part
		LifeSpan 63
	End
End

#############################################################

End