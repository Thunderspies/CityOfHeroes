#
FxInfo
Lifespan 10

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DD_Diabolique_Hit_01 100 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	ElectricityAnchor
		Type	Local
		At	Chest
		Part	:Hit_Flash_Diabolique.part
		Part	:Hit_Glow_Drone_Diabolique.part
		Part	:Hit_Streak_Drone_Diabolique.part
		Part	:Hit_Shockwave_Drone_Diabolique.part
		Part	:Hit_Smoke_Diabolique.part
		Lifespan 10
	End

End

###########################################################

End