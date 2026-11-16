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
	End

	Event
		Type	Local
		At	WepR
		Bhvr 	behaviors/GenericParticleFade.bhvr
		Part	:Sonic_Glow_Bright.part
	End
End

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	WepR
		Bhvr 	behaviors/GenericParticleFade.bhvr
		Part	:Sonic_Ripple_Large.part
		Part	:Sonic_Glow_Bright.part
		Part	:Sonic_Ripple_Noise_Linger.part
	End

	Event
		Type	Local
		At	WepL
		Bhvr 	behaviors/GenericParticleFade.bhvr
		Part	:Sonic_Ripple_Large_Left.part
		Part	:Sonic_Glow_Bright.part
		Part	:Sonic_Ripple_Noise_Linger_Left.part
	End
End

#############################################################

End