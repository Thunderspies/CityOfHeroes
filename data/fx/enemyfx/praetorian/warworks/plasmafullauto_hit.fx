#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 80

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
		Sound WW_PlasmaRifleFullAutoBurst_Attack 200 200 .8
	End

	Event
		Type	Local
		At	Chest
		Part	:Hit_Flash_Small2.part
		Part	:Hit_Glow_Small.part
		Part	:Hit_Shockwave_Small2.part
		Part	:Hit_Tendril_Small.part
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	4

	Event
		Type	Local
		At	Chest
		BhvrOverride
			StartJitter	1.0 1.0 1.0
		End
		Part	:Hit_Flash_Small2.part
		Part	:Hit_Glow_Small.part
		Part	:Hit_Shockwave_Small2.part
		Part	:Hit_Tendril_Small.part
		Lifespan 1
	End
End

#############################################################

End