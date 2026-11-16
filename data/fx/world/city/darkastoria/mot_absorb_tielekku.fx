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
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 40

		Event
			At Root
			Type Local
			Bhvr behaviors/FadeOut.bhvr
			BhvrOverride
				PositionOffset 0 5 0
				PyrRotate 90 0 90
				PyrRotateJitter 5 0 5
			End
			Part WORLD\City\DarkAstoria\Mot_Absorption_Tielekku_Lightshaft01.part
			LifeSpan 15
		End

	End

	Condition
		On Time
		Time 50

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 4 0
			End
			Part :Streaks_Explode_Detached.part
			Part :Streaks_Explode_Short.part
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 75

		Event
			At Root
			Type Local
			Bhvr behaviors/FadeOut.bhvr
			BhvrOverride
				PositionOffset 0 5 0
				PyrRotate 90 0 90
				PyrRotateJitter 5 0 5
			End
			Part WORLD\City\DarkAstoria\Mot_Absorption_Tielekku_Lightshaft02.part
			LifeSpan 30
		End

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 4 0
				PyrRotate 90 0 90
			End
			Part :Beacon_Border.part
			Part :Beacon_Swirls.part
			Part :Beacon_Swirls_Additive.part
		End

	End

	Condition
		On Time
		Time 77

		Event
			At Root
			Type Local
			Bhvr behaviors/FadeOut.bhvr
			BhvrOverride
				PositionOffset 0 5 0
				PyrRotate 90 0 90
				PyrRotateJitter 5 0 5
			End
			Part WORLD\City\DarkAstoria\Mot_Absorption_Tielekku_Lightshaft03.part
			LifeSpan 30
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

	Condition
		On Time
		Time 78

		Event
			At Root
			Type Local
			EName TPAnchor

		End

		Event
			At TPAnchor
			Type Start
			EName SpinAnchor
			Part :Beacon_Core.part
			Part :Beacon_Core_Glow.part
		End

		Event
			At TPAnchor
			Type Start
			Part :SummonRing_Flash.part
			Part :Rays_Explode.part
		End

	End

End
