#
FxInfo

	LifeSpan 200


###########################################################

Condition
	On 	Time
	Time	15
	Event
		Type	Local
		At	Root
		Sound 	DD_ConsumingBlast_Attack_01 100 100 .8
	End
End

	Condition
		On Time
		Time 15

		Event
			At WepR
			Type Local
			EName RightHandMist
			Part :FrigidLance_Hand_Snow.part
			Part :FrigidLance_Hand_Snow_Additive.part
			LifeSpan 60
		End

		Event
			At WepL
			Type Local
			EName LeftHandMist
			Part :FrigidLance_Hand_Snow.part
			Part :FrigidLance_Hand_Snow_Additive.part
			LifeSpan 60
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Head
			Type StartPositOnly
			EName SpikeBallAnchor
			Bhvr Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\ConsumingBlast_IcicleAnchor.bhvr
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\ConsumingBlast_Core.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\ConsumingBlast_Core_Subtractive.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\ConsumingBlast_Steam.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\ConsumingBlast_Rays.part
			LifeSpan 42
		End

		Event
			EName ProjectileAnchor
			At Head
			Type StartPositOnly
			Bhvr Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\ConsumingBlast_IcicleAnchor.bhvr
			LifeSpan 80
		End

	End

	Condition
		On Cycle
		Time 2

		Event
			At SpikeBallAnchor
			Type Local
			EName SpikeBall
			Bhvr Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\ConsumingBlast_Icicle.bhvr
			BhvrOverride
				StartColor 60 0 140
			End
			Geom FX_Impale_Icicle01
			Geom FX_Impale_Icicle02
			Geom FX_Impale_Icicle03
			Geom FX_Impale_Icicle04
			Geom FX_Impale_Icicle05
			LifeSpan 50
		End

	End

	Condition
		On Time
		Time 64

		Event
			At ProjectileAnchor
			Type Local
			EName Flash
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\ConsumingBlast_Shockwave.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\ConsumingBlast_Flash_Large.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 70

		Event
			At ProjectileAnchor
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\ConsumingBlast_Projectile.bhvr
			BhvrOverride
				StartColor 60 0 140
			End
			Geom FX_ConsumingBlast
		End

		Event
			At Prime
			Type Local
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\ConsumingBlast_Trail_Subtractive.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\ConsumingBlast_Trail_Subtractive_Smoke.part
			Part Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\ConsumingBlast_Trail.part
		End

	End

	Condition
		On Time
		Time 72

		Event
			At ProjectileAnchor
			Type Start
			EName Prime2
			LookAt Target
			Magnet Target
			Bhvr Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\ConsumingBlast_Projectile.bhvr
			BhvrOverride
				Scale 1.5 1.5 2
				Spin 0 0 -0.8
				StartColor 60 0 140
			End
			Geom FX_ConsumingBlast2
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

		Event
			Type Destroy
			EName Prime2
		End

	End

End
