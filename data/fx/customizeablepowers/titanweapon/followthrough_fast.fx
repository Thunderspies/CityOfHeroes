#############################################################
## FollowThrough_Fast.fx
#############################################################

FxInfo
Lifespan 100

############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TW_FollowThrough_AttackFast_01 100 100 .8
	End
End

Condition
	On	Time
	Time	16

	Event
		At C_Chest
		Type Local
		Bhvr Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 1.5 0 -2
			PyrRotate 180 -60 0
		End
		Part :Swoosh_Trail_Short.part
		Lifespan 2
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
		Part :Titan_Swoosh_Light.part
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

End
