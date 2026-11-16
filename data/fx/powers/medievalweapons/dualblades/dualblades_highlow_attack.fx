#############################################################
## DualBlades - High Low Attack
#############################################################

FxInfo

LifeSpan 120

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	Mystic
		Sound swrdwhoosh3 100.0 100.0 0.55
	End

	Event
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	:DualBlade_Swoosh1.bhvr
		Lifespan	1
	End
End

Condition
	On 	Time
	Time 	19

	Event
		Type	Local
		At	Mystic
		Sound swrdwhoosh1 100.0 100.0 0.55
	End

	Event
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	:DualBlade_Swoosh1.bhvr
		Lifespan	1
	End
End

Condition
	On 	Time
	Time 	24

	Event
		Type	Local
		At	Mystic
		Sound swrdwhoosh2 100.0 100.0 0.55
	End

	Event
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	:DualBlade_Swoosh1.bhvr
		Lifespan	1
	End
End

Condition
	On 	Time
	Time 	29

	Event
		Type	Local
		At	Mystic
		Sound swrdwhoosh3 100.0 100.0 0.55
	End

	Event
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	:DualBlade_Swoosh1.bhvr
		Lifespan	1
	End
End

Condition
	On 	Time
	Time 	34

	Event
		Type	Local
		At	Mystic
		Sound swrdwhoosh4 100.0 100.0 0.55
	End

	Event
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	:DualBlade_Swoosh1.bhvr
		Lifespan	1
	End
End

Condition
	On 	Time
	Time 	39

	Event
		Type	Local
		At	Mystic
		Sound swrdwhoosh1 100.0 100.0 0.55
	End

	Event
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	:DualBlade_Swoosh1.bhvr
		Lifespan	1
	End
End

Condition
	On 	Time
	Time 	44

	Event
		Type	Local
		At	Mystic
		Sound swrdwhoosh4 100.0 100.0 0.55
	End

	Event
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	:DualBlade_Swoosh1.bhvr
		Lifespan	1
	End
End

Condition
	On 	Time
	Time 	49

	Event
		Type	Local
		At	Mystic
		Sound swrdwhoosh1 100.0 100.0 0.55
	End

	Event
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	:DualBlade_Swoosh1.bhvr
		Lifespan	1
	End
End

Condition
	On 	Time
	Time 	54

	Event
		Type	Local
		At	Mystic
		Sound swrdwhoosh3 100.0 100.0 0.55
	End

	Event
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	:DualBlade_Swoosh1.bhvr
		Lifespan	1
	End
End

Condition
	On 	Time
	Time 	59

	Event
		Type	Local
		At	Mystic
		Sound swrdwhoosh2 100.0 100.0 0.55
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
	Time 	84

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
	Time 	15

	Event
		EName	Projectile1
		Type	Local
		At	Origin
		ChildFX	:DualBlades_HighLow_ProjectileCycle1.fx
	End
End

#############################################################

End