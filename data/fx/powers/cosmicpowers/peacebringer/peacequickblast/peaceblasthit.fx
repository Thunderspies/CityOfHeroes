#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	30

Condition
	On 	Time
	Time 	0

	Event
		EName	SwordFlash2
		Type	Local
		At	Chest
		Part	:BlastFlash.part
		Part	:BlastFlashTendrils.part
		Part	:QuickBlastHitStreaks.part
		Part	:PeaceBlastHitSparks.part
		Part	:SunFlareHitStreaks.part
		Part	:LightFlareHitStreaks.part
		Part	:PeaceBlastHitSpinners.part
		Sound peaceAOEhit 80 80 .89
		LifeSpan 10
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		300
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End

########################spinningParticles1################

	Event
		ENAME	node
		Type	Local
		At	Root
		Geom	RootToChestadjustment
		BHVR	:HitSpinFaster.bhvr
		LifeSpan 15
	End

	Event
		Type	Local
		At	node
		AltPiv	1
		BHVR	:HitSpinFasterOpposite.bhvr

		Part	:Spinner.part
		LifeSpan 15
	End

########################spinningParticles2################

	Event
		Type	Local
		At	node
		AltPiv	1
		BHVR	:HitSpinFaster.bhvr

		Part	:Spinner.part
		LifeSpan 15
	End

########################spinningParticles3################

	Event
		Type	Local
		At	node
		AltPiv	1
		BHVR	:HitSpinFaster2Opposite.bhvr

		Part	:Spinner.part
		LifeSpan 15
	End

	
End

End	


			