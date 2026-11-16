#############################################################
## Peacebringer - Quantum Flight
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	1

	Event
		Type	Local
		At	Head
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:QuantumFlight_Aura1.part
		POther	Head
	End

	Event
		Type	Local
		At	Neck
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:QuantumFlight_Aura1.part
		POther	UArmL
	End

	Event
		Type	Local
		At	Neck
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:QuantumFlight_Aura1.part
		POther	UArmR
	End

	Event
		Type	Local
		At	Waist
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:QuantumFlight_Aura2.part
		POther	Waist
	End

	Event
		Type	Local
		At	ULegL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:QuantumFlight_Aura2.part
		POther	LLegL
	End

	Event
		Type	Local
		At	ULegR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:QuantumFlight_Aura2.part
		POther	LLegR
	End

	Event
		Type	Local
		At	LLegL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:QuantumFlight_Aura2.part
		POther	FootL
	End

	Event
		Type	Local
		At	LLegR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:QuantumFlight_Aura2.part
		POther	FootR
	End

	Event
		Type	Local
		At	UArmL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:QuantumFlight_Aura1.part
		POther	LArmL
	End

	Event
		Type	Local
		At	UArmR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:QuantumFlight_Aura1.part
		POther	LArmR
	End

	Event
		Type	Local
		At	LArmL
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:QuantumFlight_Aura1.part
		POther	HandL
	End

	Event
		Type	Local
		At	LArmR
		bhvr	Behaviors/GenericParticleFade.bhvr
		part1	:QuantumFlight_Aura1.part
		POther	HandR
	End
End

#############################################################

End				