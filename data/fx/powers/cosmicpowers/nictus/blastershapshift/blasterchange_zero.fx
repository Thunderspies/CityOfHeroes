#########################################################
## Black Nova - Shapeshift
#########################################################

FxInfo

Lifespan 90

## SOUND FX #######################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	hips
		Sound Nova2 100.0 100.0 0.9
	End
End

## SHAPECHANGE FX #######################################################

Condition
	On 	Time
	Time 	0

	Event

		Type	Local
		At	hips
		Part	:NovaTendrilsBuildUp.part
		Part	:NovaTendrilsBuildUpblue.part
		Part	:NovaTendrilsBuildUpPurple.part
		lifespan 90
	End
End

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	hips
		Part	:NovaTendrils.part
		Part	:NovaTendrilsBlue.part
		Part	:NovaTendrilsPurple.part
		Part	:InfernorockBits2.part
		Part2	:Inferno.part
		Part2	:InfernoBlue.part
		Part2	:InfernoPurple.part
		lifespan 90
	End
End

## CAMERA SHAKE #######################################################

Condition
	On 	Time
	Time 	19

	Event
		Type	Local
		At	hips
		Bhvr	Behaviors/CameraShake01.bhvr
		lifespan 90
	End
End

#########################################################

End