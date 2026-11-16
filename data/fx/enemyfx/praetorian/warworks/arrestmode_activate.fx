#############################################################
## SonicPylons_Hit.fx
#############################################################

FxInfo
LifeSpan 60

#############################################################
Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	WepL
		Bhvr 	behaviors/GenericParticleFade.bhvr
		Part	:Sonic_Glow_Bright.part
		Lifespan	20
	End

	Event
		Type	Local
		At	WepR
		Bhvr 	behaviors/GenericParticleFade.bhvr
		Part	:Sonic_Glow_Bright.part
		Lifespan	20
	End
End

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	WepL
		Part	:Hands_Electricity_LargeBolts.part
		Part	:Hands_Electricity_Shockwave.part
		Lifespan	30
	End

	Event
		Type	Local
		At	WepR
		Part	:Hands_Electricity_LargeBolts.part
		Part	:Hands_Electricity_Shockwave.part
		Lifespan	30
	End

	Event
		Type	Local
		At	WepL
		Part	:Hands_Electricity.part
		Lifespan	35
	End

	Event
		Type	Local
		At	WepR
		Part	:Hands_Electricity.part
		Lifespan	35
	End
End

#############################################################

End