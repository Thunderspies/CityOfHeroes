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
		Sound WWV_Swipe_Hit 150.0 150.0 .8
		Sound WWV_Feint_Hit 150.0 150.0 .8
	End

	Event
		Type	Local
		At	Chest
		Part	:Hit_Flash_Small_Blue.part
		Part	:Hit_Star.part
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	12

	Event
		Type	Local
		At	Chest
		Part	:Hit_Flash_Small_Blue.part
		Part	:Hit_Star.part
		Part	:Hit_Glow_Blue.part
		Part	:Hit_Rings_Sharp.part
		Part	:Hit_Sparks_Blue.part
		Lifespan 1
	End
End

#############################################################

End