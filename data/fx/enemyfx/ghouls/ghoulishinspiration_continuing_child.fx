#############################################################
## GhoulishInspiration_Continuing.fx
#############################################################

FxInfo
Lifespan 15

##########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	left
		Type	Local
		At	Neck
		Bhvr	behaviors\GenericParticleFade.bhvr
		ChildFX	:Inspiration_Child_Anklet_Heartbeat.fx
		Lifespan	10
	End
End

Condition
	On	Time
	Time	4

	Event
		EName	left
		Type	Local
		At	LArmL
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			Positionoffset	-0.6 0 0
		End
		ChildFX	:Child_Inspiration_Bracelet_Heartbeat.fx
		Lifespan	10
	End

	Event
		EName	left
		Type	Local
		At	LArmR
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			Positionoffset	0.6 0 0
		End
		ChildFX	:Child_Inspiration_Bracelet_Heartbeat.fx
		Lifespan	10
	End
End

Condition
	On	Time
	Time	8

	Event
		EName	left
		Type	Local
		At	LLegL
		BhvrOverride
			Positionoffset	0 -1.25 -0.25
		End
		Bhvr	behaviors\GenericParticleFade.bhvr
		ChildFX	:Inspiration_Child_Anklet_Heartbeat.fx
		Lifespan	10
	End

	Event
		EName	left
		Type	Local
		At	LLegR
		BhvrOverride
			Positionoffset	0 -1.25 -0.25
		End
		Bhvr	behaviors\GenericParticleFade.bhvr
		ChildFX	:Inspiration_Child_Anklet_Heartbeat.fx
		Lifespan	10
	End
End

#############################################################

End