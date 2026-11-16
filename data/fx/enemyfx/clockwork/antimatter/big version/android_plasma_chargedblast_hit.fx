#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

#LifeSpan 180

#############################################################

Input
	InpName Hips
End

#############################################################


Condition
	On 	Time
	Time 	2

	Event
		Type	Local
		At	Chest

		Part	:Hit_Flash.part

		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Part	:Hit_Tendril.part
		Part	:Hit_Tendril_Core.part
		Part	:Hit_Rings.part

		Lifespan 10
	End
End

#############################################################

End