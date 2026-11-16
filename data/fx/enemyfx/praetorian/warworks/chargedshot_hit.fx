#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 60

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
		Sound WW_ChargedShot_Hit 200 200 .8
	End

	Event
		Type	Local
		At	Chest
		Part	:Hit_Flash_Large.part
		Part	:Hit_Glow.part
		Part	:Hit_Shockwave.part
		Part	:Hit_Glow_Lingering.part
		Part	:Hit_Tendril.part

		Lifespan 1
	End

	Event
		Type	Local
		At	Chest
		Part	:Hit_Rings_Blue.part
		Lifespan 5
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