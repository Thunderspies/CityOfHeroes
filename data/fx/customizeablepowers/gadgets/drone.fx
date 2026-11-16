#############################################################
## drone.fx
#############################################################

FxInfo
Flags    InheritAlpha

#################################################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Root
		Sound targetdrone_loop 30 30 .3
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 120


	End

	Event
		EName	DroneAttachment1
		Type	local
		At	Root
		Bhvr	CustomizeablePowers\Gadgets\DroneSpin.bhvr
		geom	OverHeadDummy
	End

	Event
		EName	DroneAttachment
		Type	local
		At	DroneAttachment1
		altpiv	2
		Bhvr	CustomizeablePowers\Gadgets\DroneSpinLocal.bhvr
		geom	centerdummy
	End

	Event
		EName	drone
		Type	local
		At	DroneAttachment
		Bhvr	CustomizeablePowers\Gadgets\DroneFade.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Drone
	End

	Event
		EName	lightning
		Type	local
		At	DroneAttachment
		Part	CustomizeablePowers\Gadgets\glow.part
	End

#############################################################

	Event
		Type	local
		At	Drone
		AltPiv	1
		Part1	:DroneLazerLight3.part
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

Condition
	On 	Time
	Time 	2

	Event
		EName	drone2
		Type	local
		At	DroneAttachment
		Part	CustomizeablePowers\Gadgets\ElectricityArchDroneSmall2.part
		Part	CustomizeablePowers\Gadgets\ElectricityArchContinuing.part
		Part	CustomizeablePowers\Gadgets\ElectricityArchContinuing2.part
		LifeSpan	50
	End
End

#############################################################

Condition
	On 	TriggerBits
	TriggerBits	Death

	Event
		Type	local
		At	drone
		animpiv	hair

		Part1	CustomizeablePowers\Gadgets\ElectricityGlow.part
		Part2	CustomizeablePowers\Gadgets\ElectricityGlowStar.part
		Part3	CustomizeablePowers\Gadgets\ElectricityArch4a.part
		Part4	CustomizeablePowers\Gadgets\ElectricityArchSmalla.part
		LifeSpan	30
		Sound elecballhit 80 80 .8
	End

	Event
		Type	local
		At	drone
		animpiv	hair

		Part1	CustomizeablePowers\Gadgets\ElectricityArchContinuing.part
		Part2	CustomizeablePowers\Gadgets\ElectricityArchContinuing2.part

		LifeSpan	63
	End

End


End