#########################################################
## Thermal Radiation - Melt Armor (Cast)
#########################################################

FxInfo

ClampMaxScale 1 1 1

LifeSpan 70

## TARGETTED AOE ###########################################################

Condition
	On 	Time
	Time 	26

	Event
		Type	Start
		At	Origin
		Bhvr	Behaviors\CameraShake01.bhvr
		Part	powers\firecontrol\InfernorockBits.part
		Part	powers\fireControl\InfernoExplosion.part
		Part1	powers\firecontrol\InfernoYellow2Flat.part
		Part	:InfernorockBits.part
		LifeSpan 12
	End
End

Condition
	On 	Time
	Time 	32

	Event
		Type	Start
		At	Origin
		Part	powers\fireControl\Inferno.part
		LifeSpan 15
	End

	Event
		Type	Start
		At	Origin
		Part	powers\fireControl\InfernoExplosion.part
		Part1	powers\firecontrol\InfernoYellow2Flat.part
		LifeSpan 12
	End

	Event
		Type	Start
		At	Origin
		BHVR	:SplatFast.bhvr
		splat	electricring1copy.tga
		LifeSpan 10
	End

	Event
		Type	Start
		At	Origin
		bhvr    behaviors\powers\firecontrol\Infernobubble.bhvr
		Part1	powers\firecontrol\InfernoYellow2.part
		Part2	powers\firecontrol\InfernoYellowFlame01.part
		LifeSpan 15
	End
End

#############################################################

End