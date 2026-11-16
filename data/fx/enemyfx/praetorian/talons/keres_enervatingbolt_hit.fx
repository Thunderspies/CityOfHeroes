#
FxInfo

	LifeSpan 10


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	ToV_Keres_EnervatingBolt_Hit_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			EName ElectricityAnchor
			Part :Keres_Hit_Flash.part
			Part :Keres_Hit_Glow_Drone.part
			Part :Keres_Hit_Streak_Drone.part
			Part :Keres_Hit_Shockwave.part
			Part :Keres_Hit_Smoke.part
			LifeSpan 10
		End

	End

End
