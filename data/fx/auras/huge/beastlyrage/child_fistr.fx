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
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Arm_Glow.part
		Part	:Arm_Ripples.part
		POther	LArmR
		Lifespan 	10

	End

End


Condition
	On	Time
	Time	5



	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Arm_Glow.part
		Part	:Arm_Ripples.part
		POther	WepR
		Lifespan 	10

	End
End

Condition
	On	Time
	Time	7

	Event
		Type	Local
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Hand_Glow.part
		Part	:Hand_Ripples.part
		Lifespan 	10

	End
End

##############################################################

End