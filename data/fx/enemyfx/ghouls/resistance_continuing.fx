#############################################################
## EnduranceHands.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Cycle
	Time 	40


	Event
		EName	left
		Type	Local
		At	T_LArmL
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			Positionoffset	-0.6 0 0
		End
		ChildFX	:Child_Bracelet_Heartbeat.fx
		Lifespan	60
	End

	Event
		EName	left
		Type	Local
		At	T_LArmR
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			Positionoffset	0.6 0 0
		End
		ChildFX	:Child_Bracelet_Heartbeat.fx
		Lifespan	60
	End

	Event
		EName	left
		Type	Local
		At	T_LLegL
		BhvrOverride
			Positionoffset	0 -1.25 -0.25
		End
		Bhvr	behaviors\GenericParticleFade.bhvr
		ChildFX	:Child_Anklet_Heartbeat.fx
		Lifespan	60
	End

	Event
		EName	left
		Type	Local
		At	T_LLegR
		BhvrOverride
			Positionoffset	0 -1.25 -0.25
		End
		Bhvr	behaviors\GenericParticleFade.bhvr
		ChildFX	:Child_Anklet_Heartbeat.fx
		Lifespan	60
	End

	Event
		EName	left
		Type	Local
		At	T_Neck
		Bhvr	behaviors\GenericParticleFade.bhvr
		ChildFX	:Child_Anklet_Heartbeat.fx
		Lifespan	60
	End
End

#############################################################

End