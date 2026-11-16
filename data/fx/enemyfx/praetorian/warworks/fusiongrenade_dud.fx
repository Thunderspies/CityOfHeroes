#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 30

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound WW_ChemGrenade_Attack 200 200 1
	End

	Event
		Type	Local
		At	Root
		Part	:Hit_Flash_Small2.part
		Part	:Hit_Glow_Small.part
		Part	:Hit_Shockwave_Small2.part
		Part	:Hit_Tendril_Small.part
		Lifespan 1
	End
End

#############################################################

End