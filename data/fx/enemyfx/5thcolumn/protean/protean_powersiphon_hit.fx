#############################################################
## Protean_PowerSiphon.Fx
#############################################################

FxInfo
LifeSpan	25

#########################################################
####################### AUDIO ###########################
#########################################################



#########################################################
#################### AOE Indicator ########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Sound Protean_SiphonHit_01 200 200 1
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Chest_Shockwave_Small.part
		Part	:Chest_Flash_Small.part
		Part	:Chest_Glow_Small.part
		Part	:Chest_CircleStatic.part
		LifeSpan	10
	End
End

##########################################

End