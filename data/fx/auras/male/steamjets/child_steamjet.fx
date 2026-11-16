#############################################################
## Child_SteamJet.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Steam_Jet.part
		Lifespan 5
	End

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Steam_Plume.part
		Lifespan 8
	End
End

##############################################################

End