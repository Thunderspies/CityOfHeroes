#
FxInfo

	LifeSpan 100


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DD_Impale_Explosion_01 300 100 .8
	End
End

	Condition
		On Time
		Time 0
		Repeat 20

		Event
			At WepL
			Type Start
			EName Prime2
			Bhvr behaviors\CameraShake_Strong20.bhvr
			LifeSpan 20
		End

		Event
			At WepL
			Type Start
			EName Prime2
			Bhvr :IceChunk_Hit.bhvr
			BhvrOverride
				StartColor 100 230 255
			End
			Geom tintable_FX_IceWall_01
			Geom tintable_FX_IceWall_02
			Geom tintable_FX_IceWall_03
			Geom tintable_FX_IceWall_09
			Geom tintable_FX_IceWall_10
			Geom tintable_FX_IceWall_11
			Geom tintable_FX_IceWall_12
			LifeSpan 20
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type StartPositOnly
			EName Prime2
			Bhvr :IceChunk_Hit.bhvr
			BhvrOverride
				StartColor 255 255 255
			End
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Impale_Explosion_Sparkles.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Impale_Explosion_Shards_Black.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Impale_Explosion_Shards.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Impale_Explosion_Streaks.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Impale_Explosion_Flash_Core.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Impale_Flash.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Impale_Explosion_Glow.part
			LifeSpan 5
		End

		Event
			At Chest
			Type PositOnly
			EName Prime2
			BhvrOverride
				StartColor 255 255 255
			End
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\Impale_Explosion_Smoke.part
			LifeSpan 3
		End

	End

End
