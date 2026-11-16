#
FxInfo
#Lifespan 800

###########################################################

	Condition
		On time
		time 0

		Event
			Ename PollenAnchor
			At Chest
			Type Local
			BhvrOverride
				PositionOffset 0 -0.5 0
			End
		End

		Event
			Ename Pollen
			At PollenAnchor
			Type PositOnly
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Heal_pollen.part
			Part :Heal_Stars.part
			Part :Heal_Circles.part
			Part :Heal_Glow.part
			Lifespan 30
		End
	End

	Condition
		On Cycle
		Time 80

		Event
			Ename Pollen
			At PollenAnchor
			Type PositOnly
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Heal_pollen.part
			Part :Heal_Stars.part
			Part :Heal_Circles.part
			Part :Heal_Glow.part
			Lifespan 30
		End
	End

End
