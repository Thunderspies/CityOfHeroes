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
		Sound 	DA_MotAbsorbtionFail_01 100 100 .8
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
			LifeSpan 55
		End

	End

	Condition
		On Time
		Time 0

		Event
			Anim FX_Maw_Fail
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 -12 0
				#PyrRotate 90 0 0
				Scale 0.12 0.12 0.12
				AnimScale 1
			End
			LifeSpan 65
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
			LifeSpan 45
		End

	End

	Condition
		On Time
		Time 65

		Event
			At Chest
			Type Local
			Bhvr behaviors/FadeOut.bhvr
			BhvrOverride
				FadeOutLength 1
				PositionOffset 0 0.4 0.6
			End
			Part :CoreExplosion_Flash_Initial.part
			Part :CoreExplosion_Flash_Big.part
			Part :CoreExplosion_Streaks_Big.part
			Part :CoreExplosion_Streaks_Big_Alpha.part
			Part :CoreExplosion_Shockwave_Big.part
			Part :CoreExplosion_Sparks_Big.part
			#Part :CoreExplosion_Splashes.part
			Part WORLD\City\DarkAstoria\CoreExplosion_Guts.part
			Part :CoreExplosion_Chunks.part
			LifeSpan 1
		End

	End

	Condition
		On Time
		Time 70

		Event
			At Chest
			Type Start
			BhvrOverride
				PositionOffset 0 0.4 0.6
			End
			Part :CoreExplosion_Flash_Big.part
			Part :CoreExplosion_Splatter_Hilight.part
			Part :CoreExplosion_Splatter.part
			Part :CoreExplosion_Streaks_BigB.part
		End

		Event
			At Chest
			Type Local
			BhvrOverride
				InitialVelocityJitter 0 0.4 0.6
			End
			#Part :CoreExplosion_Streaks_Big_Highlight.part
		End

	End

	Condition
		On Time
		Time 71
		Repeat 1

		Event
			At Chest
			Type Start
			BhvrOverride
				InitialVelocity 0.0 0.2 0.0
				InitialVelocityJitter 0.7 0.5 0.7
				Gravity .02
			End
			#Part :CoreExplosion_InnerSplatter_Blue.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 68

		Event
			At Chest
			Type Start
			BhvrOverride
				InitialVelocity 0.0 0.5 0.0
				InitialVelocityJitter 0.8 0.5 0.8
				Gravity .02
			End
			#Part :CoreExplosion_InnerSplatter_Blue.part
			LifeSpan 10
		End

	End

End
