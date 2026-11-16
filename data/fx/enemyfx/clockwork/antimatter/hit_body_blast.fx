#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 90

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
		At	Chest
		Part	:Electricity_Sparks.part
		Lifespan 5
	End
End

Condition
	On 	Time
	Time 	2

	Event
		Type	Local
		At	Chest

		Part	:Hit_Flash_Electricity.part
		Part	:Hit_Electricity.part
		Lifespan 10
	End

	Event
		Type	Local
		At	Root
		ChildFX	:Continuing_Body_Stunner_Short.fx
	End
End

#############################################################

End