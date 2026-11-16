#
FxInfo

	LifeSpan 150


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BM_CallHawk_Hit_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Start
			EName BatAnchor
			Flags OneAtATime
			LifeSpan 200
		End

	End

	Condition
		On Time
		Time 0

		Event
			EName RotatorAnchor

			At BatAnchor
			Type PositONly
			BhvrOverride
				PositionOffset 0 0 0
				EndScale 1 1 1
				Spin 0 0.2 0
				SpinJitter 0.08 0.02 0.08
			End
		End

		Event
			EName BatGeo

			Anim hawk
			SetState	HOVER
			At RotatorAnchor
			Type Local
			BhvrOverride
				PositionOffset 0 0 2
				PyrRotate 0 180 0
				TintGeom 1
				FadeOutLength 60
			End
			LifeSpan 50
		End

		Event
			At Chest
			Type Local
			EName SlashAnchor
			LifeSpan 40
		End

		Event
			At Chest
			Type Local
			Part :Hit_Flash.part
			Part :Hit_Glow_Additive.part
			LifeSpan 10
		End

		Event
			At SlashAnchor
			Type Local
			EName ElectricityAnchor
			Part :Hit_Flash.part
			Part :Hit_Slash.part
		End

	End

	Condition
		On Time
		Time 10

		Event
			At SlashAnchor
			Type Local
			EName ElectricityAnchor
			Part :Hit_Flash.part
			Part :Hit_Slash.part
		End

	End

	Condition
		On Time
		Time 20

		Event
			At SlashAnchor
			Type Local
			EName ElectricityAnchor
			Part :Hit_Flash.part
			Part :Hit_Slash.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 30

		Event
			At SlashAnchor
			Type Local
			EName ElectricityAnchor
			Part :Hit_Flash.part
			Part :Hit_Slash.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 40

		Event
			At SlashAnchor
			Type Local
			EName ElectricityAnchor
			Part :Hit_Flash.part
			Part :Hit_Slash.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 48

		Event
			At Chest
			Type Local
			EName ElectricityAnchor
			Part :Hit_Flash.part
			Part :Hit_Slash_Big.part
			Part :Hit_Slash.part
			LifeSpan 10
		End

		Event
			At Chest
			Type Local
			EName ElectricityAnchor
			Part :Hit_Slash_Big.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 51

		Event
			EName FlyUp

			Anim hawk
			SetState	HOVER

			At RotatorAnchor
			Type StartPositOnly
			BhvrOverride
				Gravity -0.015
				InitialVelocity 0 0.8 1
				InitialVelocityJitter 0 0.2 0
				FadeOutLength 60
			End
		End

	End

End
