#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 300

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
		Part	:Hit_Flash_Electricity_Large.part
		Part	:Electricity_Streak.part
		Part	:Hit_Flash_Electricity_Rings.part
		Part	:Hit_Electricity_Large.part
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