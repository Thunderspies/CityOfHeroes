#############################################################
## AccelerateMetabolism.fx
#############################################################

FxInfo

Lifespan 120

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	BottleRocketHookup
		Type	Local
		At	Root
		Bhvr	:RadiationPoisoningScale.bhvr
		Anim	FX_teleportbase
		sound	accel5 66 22 .9
	End

	Event
		EName 	BottleRocket03
		Type	Local
		At	BottleRocketHookup
		AnimPiv Neck
		Part1	:Poisoningtrail.part
		Part2	:PoisoningHead.part
		Part3	:RadiationHandElectrons.part
		Lifespan 75
	End

	Event
		EName 	BottleRocket04
		Type	Local
		At	BottleRocketHookup
		AnimPiv Head
		Part1	:Poisoningtrail.part
		Part2	:PoisoningHead.part
		Part3	:RadiationHandElectrons.part
		Lifespan 75
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName	HookUp
		Type	Local
		At	Root
		Geom	RootToHeadAdjustment
	End

	Event
		EName	RingsDown
		Type	Local
		At	HookUp
		Altpiv	1
		Part1	:RadiationPoisoningRingsDown.part
	End

	Event
		EName	RingsOut
		Type	Local
		At	Root
		Part1	:RadiationPoisoningRingsOut.part
	End
End

#############################################################

End