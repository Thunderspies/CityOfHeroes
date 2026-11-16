###########################################################

FxInfo
LifeSpan 200

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BPR_Enervation_VOX_01 100 100 .8
	End
End

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BP_Enervation_Attack_01 100 100 .8
	End
End

	Condition
		On Time
		Time 20

		Event
			EName SpikeBallAnchor
			At 	Chest
			Type StartPositOnly
			BhvrOverride
				InitialVelocity	0.0 0.125 0.0
				PositionOffset	0.0 12.0 0.0
			End
			Bhvr 	Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\ConsumingBlast_IcicleAnchor.bhvr

			LifeSpan 80
		End

		Event
			EName 	ProjectileAnchor
			At 	SpikeBallAnchor
			Type 	Local
			Part 	:ConsumingBlast_Core.part
			Part 	:ConsumingBlast_Core_Subtractive.part
			Part 	:ConsumingBlast_Steam.part
			Part 	:ConsumingBlast_Rays.part
			LifeSpan 45
		End

	End

	Condition
		On Cycle
		Time 2

		Event
		#	EName ProjectileAnchor
			At ProjectileAnchor
			Type Local
			Bhvr :FrigidEnervation_Icicle.bhvr
			BhvrOverride
				StartColor	135 255 255
			End
			Geom FX_Enervation_Icicle01
			Geom FX_Enervation_Icicle02
			Geom FX_Enervation_Icicle03
			Geom FX_Enervation_Icicle04
			Geom FX_Enervation_Icicle05
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 64

		Event
			At SpikeBallAnchor
			Type Local
			EName Flash
			Part :ConsumingBlast_Shockwave.part
			Part :ConsumingBlast_Flash_Large.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 70

		Event
			EName 	Prime
			At	SpikeBallAnchor
			Type 	Start
			Bhvr 	Events\EndGame\DilemmaDiabolique\Diabolique_Attacks\ConsumingBlast_Projectile.bhvr
			LookAt 	Target
			Magnet 	Target
		End

		Event
			At Prime
			Type Local
			Part :ConsumingBlast_Trail_Subtractive.part
			Part :ConsumingBlast_Trail_Subtractive_Smoke.part
			Part :ConsumingBlast_Trail.part
		End

	End

	Condition
		On Time
		Time 70

		Repeat	10

		Event
			EName SpikeBall
			At Prime
			Type Local
			Bhvr :FrigidEnervation_Icicle_Projectile.bhvr
			BhvrOverride
				StartColor	135 255 255
			End
			Geom FX_Enervation_Icicle01
			Geom FX_Enervation_Icicle02
			Geom FX_Enervation_Icicle03
			Geom FX_Enervation_Icicle04
			Geom FX_Enervation_Icicle05
			LifeSpan 50
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
		#	Geom FX_ConsumingBlast2
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

		Event
			Type	Start
			At	Target
			CHILDFX	:CHILD_FrigidEnervation_Explosion.fx
		End

	End

End
