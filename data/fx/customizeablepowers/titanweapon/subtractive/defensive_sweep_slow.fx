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
		Sound 	TW_DefensiveSweep_AttackSlow_01 100 100 .8
	End
End

Condition
	On Time
	Time 0

	Event
		At T_Chest
		Type Start
		EName ConeTarget
		BhvrOverride
			PositionOffset 0 -1 0
		End
	End
End

Condition
	On Time
	Time 58

	Event
		At C_Chest
		Type Local
		Bhvr Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 1.5 0 -2
			PyrRotate 180 -60 0
		End
		Part :Swoosh_Trail.part
		Lifespan 5
	End
End


Condition
	On Time
	Time 65

	Event
		At Chest
		Type Start
		EName Prime
		LookAt ConeTarget
		Magnet ConeTarget
		Bhvr :Swoosh_Projectile.bhvr
		Part :Titan_Swoosh_Horizontal.part
		LifeSpan 3
	End

	Event
		At Prime
		Type Local
		EName SwishParticle1
		BhvrOverride
			PyrRotate 0 180 0
		End
		Part :Air_Dust_Wave_Horiz.part
		LifeSpan 3
	End

	Event
		At Prime
		Type Local
		EName SwishParticle2
		BhvrOverride
			PyrRotate 0 180 90
		End
		Part :Air_Dust_Wave_Vert.part
		LifeSpan 3
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
		EName SwishParticle1
	End

	Event
		Type Destroy
		EName SwishParticle2
	End

End

End
