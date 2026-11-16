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
		Sound 	BP_Enervation_Hit_01 100 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	ElectricityAnchor
		Type	Local
		At	Chest
		Part	:Hit_Flash.part
		Part	:Hit_Streak_Drone.part
		Part	:Hit_Shockwave_Drone.part
		Part	:Hit_Smoke.part
		Part	:Hit_Boils.part
		Part	:Hit_Boils_Solid.part
		Lifespan 10
	End

End

###########################################################

End