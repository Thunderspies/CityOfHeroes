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
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Sound WW_Smash_01 200 200 .9
		Sound WW_Smash_02 200 200 .9
		Sound WW_Smash_03 200 200 .9
	End

	Event
		Type	Local
		At	Chest
		Part	:Hit_Shockwave_Small_Yellow.part
		Part	:Hit_Flash_Small_Yellow.part
		Part	:Hit_Rings_Small_Yellow.part
		Part	:Hit_Sparks_Large.part
		Lifespan 1
	End
End



#############################################################

End