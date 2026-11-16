#############################################################
## DefensiveSweep_Fast.fx
#############################################################

FxInfo
Lifespan 100

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TW_DefensiveSweep_AttackFast_01 100 100 .8
	End
End

Condition
	On Time
	Time 15

	Event
		At C_Chest
		Type Local
		Bhvr Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 1.5 0 -2
			PyrRotate 180 -60 0
		End
		Part :Swoosh_Trail_Short.part
		Lifespan 5
	End
End

Condition
	On	Time
	Time	20

	Event
		At T_Chest
		Type Start
		EName ConeTarget
		BhvrOverride
			PositionOffset 0 -1 0
		End
	End

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
		Part CustomizeablePowers\TitanWeapon\Air_Dust_Wave_Horiz.part
		LifeSpan 3
	End

	Event
		At Prime
		Type Local
		EName SwishParticle2
		BhvrOverride
			PyrRotate 0 180 90
		End
		Part CustomizeablePowers\TitanWeapon\Air_Dust_Wave_Vert.part
		LifeSpan 3
	End
End

Condition
	ON	PrimeHit

	Event
		EName	Prime
		Type	DESTROY
	End

	Event
		EName	SwishParticle
		Type	DESTROY
	End

	Event
		EName	ConeTarget
		Type	DESTROY
	End

End

#################

End
