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
		sound Plasma_Charged_Blast_Hit 200 200 .8
	End

	Event
		Type	Local
		At	Chest

		Part	:Hit_Flash_Large.part

		Lifespan 1
	End

	Event
		Type	Local
		At	Chest
		Part	:Hit_Tendril.part
		Part	:Hit_Tendril_Core.part
		Part	:Hit_Rings.part
		Part	:Hit_Rings_Blue.part

		Lifespan 10
	End
End

#############################################################

End