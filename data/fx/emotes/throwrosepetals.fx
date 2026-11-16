#############################################################
## Thrown Confetti FX
#############################################################

FxInfo

Lifespan 30

## ROSE PETALS - WHITE ###########################################################

Condition
	On 	Time
	Time 	0
	Repeat 		16
	RepeatJitter	4

	Event
		Type	Start
		At	Mystic
		Bhvr	:RosePetal.bhvr
		BhvrOverride
			StartColor	255 255 255
		End
		geom	FX_RosePetal1
		geom	FX_RosePetal2
		geom	FX_RosePetal3
		geom	FX_RosePetal4
	End
End

## ROSE PETALS - PINK ###########################################################

Condition
	On 	Time
	Time 	2
	Repeat 		12
	RepeatJitter	6

	Event
		Type	Start
		At	Mystic
		Bhvr	:RosePetal.bhvr
		BhvrOverride
			StartColor	248 128 255
		End
		geom	FX_RosePetal1
		geom	FX_RosePetal2
		geom	FX_RosePetal3
		geom	FX_RosePetal4
	End
End

## ROSE PETALS - RED ###########################################################

Condition
	On 	Time
	Time 	4
	Repeat 		12
	RepeatJitter	6

	Event
		Type	Start
		At	Mystic
		Bhvr	:RosePetal.bhvr
		BhvrOverride
			StartColor	96 0 0
		End
		geom	FX_RosePetal1
		geom	FX_RosePetal2
		geom	FX_RosePetal3
		geom	FX_RosePetal4
	End
End

#############################################################

End