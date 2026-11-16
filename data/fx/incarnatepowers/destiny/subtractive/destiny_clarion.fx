#############################################################
## Disruption Field
#############################################################

FxInfo
Lifespan 100

#############################################################

#Condition
#	On	Time
#	Time 	7
#
#	Event
#		Type 	Local
#		At	Origin
#		Sound sonichands 80 80 .8
#	End
#End
#
#Condition
#	On	Time
#	Time 	17
#
#	Event
#		Type 	Local
#		At	Origin
#	#	Sound sonic2_loop 100.0 100.0 .77
#		bhvr	Behaviors\soundFade3.bhvr
#		Flags	PowerLoopingSound
#		Lifespan 30
#	End
#End
#
#Condition
#	On 	Time
#	Time 	30
#
#	Event
#		Type	Local
#		At	Chest
#	#	Sound sonic3 70 70 .75
#	End
#End

## HANDS ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		ChildFX	IncarnatePowers\Incarnate_Gather.fx
		Lifespan	30
	End

	Event
		Type	Local
		At	Root
		Sound 	Incarnate_Clarion_01 100 100 .8
	End

	Event
		Type	StartPositOnly
		At	Root
		Part	:Clarion_Rings.part
		Part	:Clarion_Rings_Highlight.part
		LifeSpan 45
	End

End

Condition
	On	Time
	Time	10


	Event
		Type	StartPositOnly
		At	Root
		Part	:Clarion_Rings_Pillar.part
		Part	:Clarion_Light_Pillar.part
		LifeSpan 45
	End

	Event
		Type	StartPositOnly
		At	Root
		Part	:Clarion_WhiteNoise_Fill.part
		LifeSpan 70
	End


	Event
		Type	StartPositOnly
		At	Root
		Part	:Clarion_WhiteNoise.part
		LifeSpan 60
	End
End

Condition
	On	Time
	Time	25

	Event
		Type	StartPositOnly
		At	Root
		Part	:Clarion_Rings_Perimeter.part
		Part	:Clarion_Rings_Perimeter_Subtractive.part

		LifeSpan 50
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	StartPositOnly
		At	Root
		Bhvr	:Clarion_Bubble_Expand.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Incarnate_Clarion_Bubble_Dark
		LifeSpan 10
	End

End

#############################################################

End