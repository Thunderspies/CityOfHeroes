#
FxInfo

	LifeSpan 120


###########################################################

	Condition
		On Time
		Time 0

		Event
			At T_Chest
			Type Start
			EName Prime
			LookAt Chest
			Magnet Chest
			Bhvr CustomizeablePowers\BeastMastery\CallSwarm_CC_Projectile.bhvr
			BhvrOverride
				InitialVelocity 0 -0.2 0
				TrackRate 2
			End
		End

		Event
			At T_Chest
			Type Start
			EName Prime2
			LookAt Chest
			Magnet Chest
			Bhvr CustomizeablePowers\BeastMastery\CallSwarm_CC_Projectile.bhvr
			BhvrOverride
				InitialVelocity 0.0 0.2 0.0
				TrackRate 2
			End
		End

		Event
			At Prime
			Type Local
			EName Trail
			BhvrOverride
				PositionOffset 0.2 0.0 0
			End
			Part :Tintable_Ivy_Trail.part
		End

		Event
			At Prime2
			Type Local
			EName Trail2
			BhvrOverride
				PositionOffset -0.2 0.0 0.0
			End
			Part :Tintable_Ivy_Trail.part
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

	Condition
		On Time
		Time 5

		Event
			EName HitAnchor
			At Chest
			Type Local
			ChildFx :Custom_IvyBody_Male_Activation.fx
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Ivy_Leaf02.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Heal_Glow.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Ivy_Leaf01.part
			#LifeSpan 100
		End

	End

	Condition
		On Cycle
		Time 30

		Event
			At HitAnchor
			Type PositOnly
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Heal_Stars.part
			Part CustomizeablePowers\PlantControl\NatureAffinity\Tintable_Heal_Circles.part
			LifeSpan 15
		End

	End

End
