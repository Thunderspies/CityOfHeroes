#############################################################
## Regrowth_Hit.fx
#############################################################
FxInfo
Lifespan 120

###########################################################


	Condition
		On time
		time 5

		Event
			Ename PollenAnchor
			At Chest
			Type Local
			BhvrOverride
				PositionOffset 0 -0.5 0
			End
		End

		Event
			Ename Pollen1
			At PollenAnchor
			Type PositOnly
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Heal_pollen.part
			Part :Heal_Stars.part
			Part :Heal_Circles.part
			Lifespan 30
		End
	End

	Condition
		On Cycle
		Time 45

		Event
			Ename Pollen2
			At PollenAnchor
			Type PositOnly
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Heal_pollen.part
			Part :Heal_Stars.part
			Part :Heal_Circles.part
			Lifespan 30
		End
	End

#############################################################
End
