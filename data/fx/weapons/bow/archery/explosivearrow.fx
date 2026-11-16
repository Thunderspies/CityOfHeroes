#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 1 1 1

Lifespan 90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		BHVR	Behaviors\RootToChestAdjustment.bhvr
		Part	:Inferno.part
		Sound debrisExplo 100 100 .88
		Lifespan 25
	End

	Event
		Type	Local
		At	Origin
		Bhvr	:CameraShake01.bhvr
		Part	:InfernorockBits.part
		Part	:InfernoExplosion.part
		Part	:InfernoExplosionFlat.part
		Lifespan 15
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Origin
		Bhvr	:Inferno.bhvr
	End

	Event
		Type	Local
		At	RadiusFxScale
		bhvr    :Infernobubble.bhvr
		Part1	:InfernoYellow2.part
		Part2	:InfernoYellowFlame01.part
		Lifespan 57
	End
End

Condition
	On 	Time
	Time 	1

	Event
		EName   RadiusFxScale2
		Type	local
		At	Origin
		Bhvr	:Inferno.bhvr
	End

	Event
		Type	Local
		At	RadiusFxScale2
		Part1   :Infernoflash.part
	End
End

#############################################################

End