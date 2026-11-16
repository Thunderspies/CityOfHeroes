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
		Part	:InfernorockBits.part
		Part	:InfernoExplosion.part
		Part	:InfernoYellow2Flat.part
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
		Part	:Inferno.part
		LifeSpan 15
	End

	Event
		Type	Start
		At	Origin
		Part	:InfernoExplosion.part
		Part1	:InfernoYellow2Flat.part
		LifeSpan 12
	End

	Event
		Type	Start
		At	Origin
		BHVR	V_COV\Powers\ThermalRadiation\splatFast.bhvr
		splat	electricring1copy.tga
		LifeSpan 10
	End

	Event
		Type	Start
		At	Origin
		bhvr    behaviors\powers\firecontrol\Infernobubble.bhvr
		Part1	:InfernoYellow2.part
		Part2	:InfernoYellowFlame01.part
		LifeSpan 15
	End
End

#############################################################

End