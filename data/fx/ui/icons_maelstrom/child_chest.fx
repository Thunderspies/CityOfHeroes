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
		At	Root
		BhvrOverride
			PositionOffset	0 7.5 -0.5
		End
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Heartbeat_Glow.part
		Part	:Heartbeat_Flash.part
		Part	:Heartbeat_Ring.part
		Lifespan 	10
	End
End


Condition
	On	Time
	Time	8

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 7.5 -0.5
		End
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Heartbeat_Flash_Small.part
		Lifespan 	10
	End
End

##############################################################

End