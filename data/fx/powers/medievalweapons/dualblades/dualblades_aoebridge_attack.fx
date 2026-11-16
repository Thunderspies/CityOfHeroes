#############################################################
## DualBlades - AOE Bridge Attack
#############################################################

FxInfo

LifeSpan	180

#############################################################

Condition
	On 	Time
	Time 	14

	Event
		Type	Start
		At	Root
		Bhvr	:DualBlade_AOESphereScaler1.bhvr
		Geom	SuperStrengthBubble #GEO_WhiteSphere
		Lifespan	2
	End
End

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
		At	Root
		bhvr	:DualBlade_AOESwoosh1.bhvr
		Geom	DualBladesAOEBridgeSwoosh
		Lifespan	2
	End
End

#############################################################

Condition
	On 	Time
	Time 	31

	Event
		Type	Local
		At	Hips
		Sound Whoosh5 100.0 100.0 0.3
	End


	Event
		Type	Start
		At	Hips
		bhvr	:DualBlade_AOESwoosh2.bhvr
		Geom	DualBladesAOEBridgeSwoosh2
		Lifespan	2
	End
End

Condition
	On 	Time
	Time 	36

	Event
		Type	Local
		At	Hips
		Sound swrdwhoosh1 100.0 100.0 0.3
	End

	Event
		Type	Local
		At	Hips
		Sound twirly 100.0 100.0 0.25
	End
	
	Event
		Type	Start
		At	Hips
		bhvr	:DualBlade_AOESwoosh2.bhvr
		Geom	DualBladesAOEBridgeSwoosh2
		Lifespan	2
	End
End

Condition
	On 	Time
	Time 	41

	Event
		Type	Local
		At	Hips
		Sound swrdwhoosh2 100.0 100.0 0.3
	End
	
	Event
		Type	Start
		At	Hips
		bhvr	:DualBlade_AOESwoosh2.bhvr
		Geom	DualBladesAOEBridgeSwoosh2
		Lifespan	2
	End
End

Condition
	On 	Time
	Time 	46

	Event
		Type	Local
		At	Hips
		Sound swrdwhoosh4 100.0 100.0 0.4
	End

	Event
		Type	Start
		At	Hips
		bhvr	:DualBlade_AOESwoosh2.bhvr
		Geom	DualBladesAOEBridgeSwoosh2
		Lifespan	2
	End
End


Condition
	On 	Time
	Time 	51

	Event
		Type	Local
		At	Hips
		Sound swrdwhoosh2 100.0 100.0 0.4
	End
	
	Event
		Type	Start
		At	Hips
		bhvr	:DualBlade_AOESwoosh2.bhvr
		Geom	DualBladesAOEBridgeSwoosh2
		Lifespan	2
	End
End

#############################################################

End