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
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Heartbeat_Glow.part
		Part	:Heartbeat_Rays.part

		Lifespan 	10
	End
End


Condition
	On	Time
	Time	8

	Event
		Type	Local
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Arm_Glow.part
		POther	LArmL
		Lifespan 	10
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Arm_Glow.part
		POther	LArmR
		Lifespan 	10

	End
End

##############################################################

End