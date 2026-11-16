############################################################
##	Fireweapon
###########################################################



FxInfo

Flags	InheritAnimScale

Lifespan	120

#################################################################################
#############################  P R I M E    H I T  ##############################
#################################################################################

Condition
	On 	Time
	Time	5
	Event
		Type	Local
		At	Root
		Sound 	BPB_FrigidBurst_Attack_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0


	Event
		EName 	Smoke02
		Type	Local
		At	Fore_FootR
		Part	:FrigidSlash_Claws_Smoke.part
		Part	:FrigidSlash_Claws_Subtractive.part
		POther	Fore_ToeR
		Lifespan	70
	End

	Event
		EName 	Smoke01
		Type	Local
		At	Fore_LLegR
		Part	:FrigidSlash_Claws_Subtractive_Light.part
		POther	Fore_FootR
		Lifespan	70
	End

End

Condition
	On	Time
	Time	5

	Event
		EName 	Smoke01
		Type	Local
		At	Fore_FootL
		Part	:FrigidSlash_Claws_Smoke.part
		Part	:FrigidSlash_Claws_Subtractive.part
		POther	Fore_ToeL
		Lifespan	70
	End

	Event
		EName 	Smoke02
		Type	Local
		At	Fore_LLegL
		Part	:FrigidSlash_Claws_Subtractive_Light.part
		POther	Fore_FootL
		Lifespan	70

	End
End

Condition
	On	Time
	Time	10


	Event
		EName 	ProjectileAnchor
		Type	Local
		At	Root
		BhvrOverride
			InitialVelocity	0.0 0.3 0.0
			PositionOffset	0.0 4.0 4.0
		End
		Lifespan	30

	End

	Event
		EName 	BallAnchor
		Type	Local
		At	ProjectileAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:FrigidBurst_Core.part
		Part	:FrigidBurst_Particles.part
		Part	:FrigidBurst_Smoke.part
		Part	:FrigidBurst_Boils.part
		Lifespan	20

	End
End

Condition
	On	Time
	Time	40

	Event
		EName 	Prime
		Type	Start
		At	ProjectileAnchor
		Bhvr	:FrigidBurst_Projectile.bhvr
		Part	:FrigidBurst_Core.part
		Part	:FrigidBurst_Core_Trail.part

		Part	:FrigidBurst_Particles_Trail.part
		Part	:FrigidBurst_Smoke_Trail.part
		Part	:FrigidBurst_Boils_Trail.part
		Part	:FrigidBurst_Boils.part

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
		Sound 	BP_FrigidGrasp_Hit_01 100 100 .5
	End
End

#################################################################################

End