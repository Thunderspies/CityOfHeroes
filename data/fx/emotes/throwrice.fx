#############################################################
## Thrown Confetti FX
#############################################################

FxInfo

Lifespan 30

## PARTICLE RICE ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Mystic
		Part1	:Rice1.part
		Part1	:Rice2.part
		LifeSpan 	5
	End
End

### GEO RICE ###########################################################
#
#Condition
#	On 	Time
#	Time 	0
#	Repeat 		12
#	RepeatJitter	4
#
#	Event
#		Type	Start
#		At	Mystic
#		Bhvr	:Rice.bhvr
#		BhvrOverride
#			StartColor	255 255 255
#		End
#		geom	FX_Rice
#	End
#End
#
#Condition
#	On 	Time
#	Time 	1
#	Repeat 		12
#	RepeatJitter	4
#
#	Event
#		Type	Start
#		At	Mystic
#		Bhvr	:Rice.bhvr
#		BhvrOverride
#			StartColor	255 255 255
#		End
#		geom	FX_Rice
#	End
#End
#
#Condition
#	On 	Time
#	Time 	3
#	Repeat 		12
#	RepeatJitter	4
#
#	Event
#		Type	Start
#		At	Mystic
#		Bhvr	:Rice.bhvr
#		BhvrOverride
#			StartColor	255 255 255
#		End
#		geom	FX_Rice
#	End
#End
#
#Condition
#	On 	Time
#	Time 	5
#	Repeat 		12
#	RepeatJitter	4
#
#	Event
#		Type	Start
#		At	Mystic
#		Bhvr	:Rice.bhvr
#		BhvrOverride
#			StartColor	255 255 255
#		End
#		geom	FX_Rice
#	End
#End

#############################################################

End