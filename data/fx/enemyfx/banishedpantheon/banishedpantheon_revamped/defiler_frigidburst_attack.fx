############################################################
##	Fireweapon
###########################################################

FxInfo
Flags	InheritAnimScale
Lifespan	200

#################################################################################
#############################  P R I M E    H I T  ##############################
#################################################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BPD_FrigidBurst_VOX_01 100 100 .8
	End
End

Condition
	On 	Time
	Time	15
	Event
		Type	Local
		At	Root
		Sound 	BPD_FrigidBurst_Attack_01 200 100 .8
	End
End
Condition
	On	Time
	Time	0

	Event
		EName 	Smoke01
		Type	Local
		At	HandL
		BhvrOverride
			PositionOffset	-2.0 0.0 0.0
		End
		Part	:Defiler_FrigidSlash_Claws_SmokeL.part
		Part	:Defiler_FrigidSlash_Claws_SubtractiveL.part
		POther	HandL
		Lifespan 85
	End


	Event
		Type	Local
		At	HandR
		BhvrOverride
			PositionOffset	2.0 0.0 0.0
		End
		Part	:Defiler_FrigidSlash_Claws_Smoke.part
		Part	:Defiler_FrigidSlash_Claws_Subtractive.part
		POther	HandR
		Lifespan 85
	End
End

Condition
	On	Time
	Time	20


	Event
		EName 	ProjectileAnchor
		Type	Start
		At	Chest
		BhvrOverride
			InitialVelocity	0.0 0.05 0.0
			PositionOffset	0.0 4.0 0.0
		End
		Lifespan	80

	End

	Event
		EName 	BallAnchor
		Type	Local
		At	ProjectileAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:FrigidBurst_Core.part
		Part	:FrigidBurst_Particles_Large.part
		Part	:FrigidBurst_Smoke_Large.part
		Part	:FrigidBurst_Boils_Large.part
		Lifespan	40

	End
End

Condition
	On	Time
	Time	70

	Event
		EName 	Prime
		Type	Start
		At	ProjectileAnchor
		Bhvr	:FrigidBurst_Projectile.bhvr
		Part	:FrigidBurst_Core.part
		Part	:FrigidBurst_Core_Trail.part
		Part	:FrigidBurst_Particles_Trail.part
		Part	:FrigidBurst_Smoke_Trail_Large.part
		Part	:FrigidBurst_Boils_Trail_Large.part
		Part	:FrigidBurst_Boils_Large.part
		Part	:FrigidBurst_Smoke_Large.part
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End

	Event
		EName	Burst
		Type	StartPositOnly
		At	Target
		CHILDFX	:CHILD_FrigidBurst_Explosion.fx
	End
End

#################################################################################

End