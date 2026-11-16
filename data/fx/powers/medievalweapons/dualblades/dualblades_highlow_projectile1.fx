#############################################################
## DualBlades - High Low Attack
#############################################################

FxInfo

LifeSpan 5

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	ProjectileTargetOffset
		Type	Start
		At	T_Root
		BhvrOverride
			PositionOffset		0 4 0
			StartJitter		5.0 0.0 5.0
		End
		##Geom	GEO_FX_TestSphere
	End

	Event
		EName	ProjectileStartOffset
		Type	Start
		At	Chest
		##Geom	GEO_FX_TestSphere
		LookAt	ProjectileTargetOffset
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	Projectile1
		Type	Local
		At	ProjectileStartOffset
		BhvrOverride
			PyrRotateJitter		0 0 45
		End
		part	:DualBlades_Projectile1.part
		pMagnet	ProjectileTargetOffset
	End
End

#############################################################

End