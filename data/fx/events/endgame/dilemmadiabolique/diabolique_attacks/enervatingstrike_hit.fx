#
FxInfo

Lifespan 60
###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DD_EnervatingStrike_Hit_01 100 100 .5
	End
End

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BP_FrigidSlash_Attack_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			EName	SlashAnchor
			At Chest
			Type Local
			LifeSpan 40
		End

		Event
			At Chest
			Type Local
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Hit_Flash.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Hit_Glow_Additive.part
			LifeSpan 10
		End

		Event
			At SlashAnchor
			Type Local
			EName ElectricityAnchor
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Hit_Flash.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Hit_Slash.part
			LifeSpan 10
		End

	End

	Condition
		On Cycle
		Time 10

		Event
			At SlashAnchor
			Type Local
			EName ElectricityAnchor
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Hit_Flash.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Hit_Slash.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 40

		Event
			At Chest
			Type Local
			EName ElectricityAnchor
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Hit_Glow_Drone2.part
			Part :Hit_Shockwave_Drone.part
			LifeSpan 10
		End

	End

End
