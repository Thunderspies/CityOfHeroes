#############################################################
## Child_BulletRain_Hit.fx
#############################################################

FxInfo

Lifespan 49

## VAPOR TRAILS ###########################################################

Condition
	On 	Cycle
	Time 	5
	Repeat	3
	RepeatJitter	1

	Event
		Ename	Projectile1
		Type	Start
		At	Origin
		Bhvr	Behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset		0 30 0
			StartJitter 		15 0 15
			InitialVelocity		0 -5 0
		End
		##Geom	Bullet01
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail01.part
		Part	WEAPONS\DualPistols\BulletTrail_VaporTrail03.part
		Lifespan 10
	End
End

#############################################################

End