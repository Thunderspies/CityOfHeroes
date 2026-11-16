#############################################################
## DualBlades - Moderate Opening Attack
#############################################################

FxInfo

LifeSpan 120

#############################################################

Condition
	On 	Time
	Time 	14

	Event
		Type	Local
		At	Mystic
		Sound Whoosh5 100.0 100.0 0.25
	End

	Event
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	:DualBlade_Swoosh1.bhvr
		Lifespan	1
	End
End

#############################################################

Condition
	On 	Time
	Time 	24

	Event
		Type	Local
		At	Mystic
		Sound Whoosh5 100.0 100.0 0.25
	End

	Event
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	:DualBlade_Swoosh1.bhvr
		Lifespan	1
	End
End

#############################################################

Condition
	On 	Time
	Time 	29

	Event
		Type	Local
		At	Mystic
		Sound Whoosh5 100.0 100.0 0.7
	End
End

#############################################################

End