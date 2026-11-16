#############################################################
## Android_Particle_Stunner.fx
#############################################################

FxInfo
#LifeSpan	200

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event	Ename	TargetNode
		Type	Local
		At	Target
		BhvrOverride
			PositionOffset	0 5 0
			StartJitter	20 5 20
			TrackRate	1.0
		End
	#	Geom	GEO_FX_TestSphere

		Magnet	Target
	End

	Event
		Ename	Prime
		Type	Start
		At 	Head
		BhvrOverride
#			InitialVelocity		0 0.5 0
#			InitialVelocityJitter 	1.0 0.5 1.0
			TrackRate	3.5
		End
		Part	:Projectile_Particle_TrailCore.part
		Part	:Projectile_Particle_Head.part
		Part	:Projectile_Particle_Tail.part
		Part	:Projectile_Particle_Tail_Stretched.part
		Magnet	TargetNode
		LookAT	TargetNode
		Lifespan 300
	End
End


Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End

#############################################################################

End