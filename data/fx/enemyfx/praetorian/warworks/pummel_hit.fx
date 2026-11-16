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
		At	Root
		Sound WW_Pummel_Hit 200 200 .8
	End

	Event
		Type	Local
		At	Chest
		Part	:Hit_Glow_Lingering.part

		Part	:Hit_Shockwave_Small_Yellow.part
		Part	:Hit_Flash_Small_Yellow.part
		Part	:Hit_Rings_Small_Yellow.part
		Part	:Hit_Sparks_Large.part
		Lifespan 1
	End

	Event
		Type	Local
		At	Chest
		Part	:Hit_Rings_Smoke.part
		Lifespan 10
	End
End

#############################################################

End