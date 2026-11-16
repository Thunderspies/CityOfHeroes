#############################################################
## Child_SteamJet_Small.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		ChildFX	:BeastlyRage_Eyes.fx
		Lifespan 	10

	End
End

Condition
	On	Time
	Time	11

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		ChildFX	:Child_SteamJet_Small.fx
		Lifespan 	30

	End
End

##############################################################

End