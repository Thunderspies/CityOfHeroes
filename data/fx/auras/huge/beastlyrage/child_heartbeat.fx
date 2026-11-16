#############################################################
## Child_SteamJet_Small.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0.0 0.35 0.6
		End
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Heartbeat_Flash.part
		Part	:Heartbeat_Ring.part
		Lifespan 	10
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0.0 0.35 0.6
		End
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Heartbeat_Flash.part
		Part	:Heartbeat_Ring.part
		Lifespan 	10
	End
End

##############################################################

End