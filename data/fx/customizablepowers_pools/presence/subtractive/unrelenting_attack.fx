#
FxInfo

	LifeSpan 200


###########################################################

	Condition
		On Time
		Time 5

		Event
			At Root
			Type Local
			Sound Awakened_PsychicRoar_Attack_01 100 100 0.8
		End
	End

	Condition
		On Time
		Time 2

		Event
			At Root
			Type StartPositOnly
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Unrelenting_Streaks_Central.part
			LifeSpan 35
		End

		Event
			At Root
			Type StartPositOnly
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Unrelenting_Light_Central.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 5

		Event
			At Chest
			Type StartPositOnly
			Bhvr behaviors/GenericParticleFade.bhvr
			Part :Tintable_Heal_Circles.part
			Part :Heartbeat_Glow.part
			Part :Heartbeat_Flash.part
			ChildFX IncarnatePowers\Hybrid\Subtractive\Hybrid_Control_Child.fx
			LifeSpan 45
		End

	End

End
