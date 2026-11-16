#############################################################
## ForceBubbleHands.fx
#############################################################

FxInfo

Lifespan 42

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Posit
		At	WepL
		Part1	CustomizeablePowers\ForceField\HandGlows.part
		Sound forcehands7 100.0 100.0 .6
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	Start
		At	WepL
		Bhvr	CustomizeablePowers\ForceField\ForceBubbleHands2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	tintable_FX_ForceBubble
		Lifespan 9
	End
End

#############################################################

End