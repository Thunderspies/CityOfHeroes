#
FxInfo

	LifeSpan 50


###########################################################

	Condition
		On Time
		Time 0
	Event
		Type	Local
		At	Root
		Sound 	Fireworks_Launch_01 100 100 .8
		Sound 	Fireworks_Launch_02 100 100 .8
		Sound 	Fireworks_Launch_03 100 100 .8
	End
	Event
		Type	Local
		At	Root
		Sound 	Fireworks_Rocket_01 100 100 .5
		Sound 	Fireworks_Rocket_02 100 100 .5
		Sound 	Fireworks_Rocket_03 100 100 .5
	End
		Event
			At WepR
			Type Local
			BhvrOverride
				PositionOffset	0.0 0.0 1.0
			End
			Part	:Tube_Flash.part

		End

		Event
			At WepR
			Type Local
			EName Offset
			Bhvr :Fireworks_Rocket_Projectile.bhvr
		End

		Event
			At Offset
			Type Local
			EName Spinner
			BhvrOverride
			#	PositionOffset 0.0 0.0 0.0
				StartJitter 0.0 0.0 0.1
			End
			#LifeSpan 17
		End

		Event
			At Spinner
			Type Local
			BhvrOverride
			#	Spin 0.0 1.0 0.0
				SpinJitter 0.0 0.35 0.0
			End
			Part :Rocket_Trail_Sparks.part
			Part :Rocket_Head_Sparks_Long.part
			LifeSpan 23
			LifeSpanJitter 3
		End

		Event
			At Spinner
			Type Local
			BhvrOverride
			#	Spin 0.0 1.0 0.0
				SpinJitter 0.0 0.35 0.0
			End
			Part :Rocket_Head_Sparks.part
			Part :Rocket_Head_Trail_Glow.part
			Part :Rocket_Trail_Smoke.part
			Part :Rocket_Head_Main.part
			LifeSpan 18
			LifeSpanJitter 3
		End

	End

End
