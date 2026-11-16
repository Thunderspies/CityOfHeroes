#
FxInfo

	LifeSpan 50


###########################################################

Condition
	On 	Time
	Time	8
	Event
		Type	Local
		At	Root
		Sound 	DA_TielekkuRanged01_Attack_01 100 100 .8
	End
End


	Condition
		On Time
		Time 10

		Event
			At Root
			Type Local
			#Sound CoL_LightStaffBolt_Attack_01 100 100 0.8
		End

	End

	Condition
		On Time
		Time 0

		Event
			At WepR
			Type Local
			Ename HandAnchor
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.3 0 0
			End
			Part WORLD\City\DarkAstoria\HandRunes01.part
			Part WORLD\City\DarkAstoria\HandRunes02.part
			Part :ProjectileInitFlash.part
		End

		Event
			At HandAnchor
			Type Local
			BhvrOverride
				PyrRotate -45 0 110
				Spin 0 -0.1 0
			End
			Part WORLD\City\DarkAstoria\HandRunes_Circle.part
		End

	End

	Condition
		On Time
		Time 13

		Event
			At WepR
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr behaviors\Braz\Projectile_FastWithUp_02.bhvr
			Part :Beam.part
			Part :Beam_EdgeNoise.Part
			Part :Beam_Particles.part
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

	End

End
