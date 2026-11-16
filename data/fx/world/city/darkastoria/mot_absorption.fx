#
FxInfo
Lifespan 300

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DA_MotAbsorbtionCloud_01 100 100 .35
	End
End


Condition
	On 	Time
	Time	30
	Event
		Type	Local
		At	Root
		Sound 	DA_MotAbsorbtion_01 100 100 .8
		Sound 	DA_MotAbsorbtion_02 100 100 .8
		Sound 	DA_MotAbsorbtion_03 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Bhvr behaviors/FadeOut.bhvr
			Part WORLD\City\DarkAstoria\Mot_Absorption_DustSwirl_inner.part
			Part WORLD\City\DarkAstoria\Mot_Absorption_DustSwirl_Add.part
			Part WORLD\City\DarkAstoria\Mot_Absorption_DustSwirl.part
			LifeSpan 90
		End

	End

	Condition
		On Time
		Time 30

		Event
			Anim FX_Maw
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 -12 0
				Scale 0.12 0.12 0.12
				AnimScale .6
			End
			LifeSpan 80
		End

		Event
			At Root
			Type Local
			Bhvr behaviors/FadeOut.bhvr
			BhvrOverride
				Spin 0 0.05 0
			End
			Part WORLD\City\DarkAstoria\Mot_Absorption_DustRise_Up.part
			Part WORLD\City\DarkAstoria\Mot_Absorption_DustIdle.part
			Part WORLD\City\DarkAstoria\Mot_Absorption_DustRise.part
			LifeSpan 50
		End

	End

	Condition
		On Time
		Time 50

		Event
			At Chest
			Type Local
			Flags AdoptParentEntity
			Bhvr behaviors/FadeOut.bhvr
			BhvrOverride
				FadeOutLength 1
				PositionOffset 0 0.4 0.6
			End
			LifeSpan 1
		End

	End

	Condition
		On Time
		Time 95

		Event
			At Root
			Type Local
			Bhvr behaviors/FadeOut.bhvr
			Part WORLD\City\DarkAstoria\Mot_Absorption_DustDive_Add.part
			Part WORLD\City\DarkAstoria\Mot_Absorption_DustRise.part
			Part :Mot_Absorption_DustDive.part
		End

	End

End
