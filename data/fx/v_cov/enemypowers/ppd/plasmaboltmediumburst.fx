#########################################################
##	
##

FxInfo

LifeSpan  400

##################################


Condition
	On	Time
	Time	10
	

	Event
		ENAME   p2
		Type	Local 
		At	LarmL
		BhvrOverride
			PositionOffset	-1.6 -.05 -.35
		End
	End


	Event
		Type	Local
		At 	Origin
		Sound RobotHeavyLazerBurst2 100 100 .9
	End
End

Condition
	On 	Time
	Time 	29

	Event
		ENAME   p1
		Type	Local 
		At	p2

		Geom	centerdummy
	End

	Event
		Type	Local 
		At	p2

		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:PlasmaBlastFlash.part
		Part	:PlasmaBlastStar.part
		
		Part	:PlasmaFire.part
		
		LifeSpan	5
	End

	Event
		EName 	Prime
		Type	Start
		At	p1
		Altpiv 1
		
		BHVR	:PlasmaBoltProjectile.bhvr
		bhvrOverride
			InitialVelocity		-0.3 0.0 0.0
			InitialVelocityJitter	0.1 0.1 0.0
		End

		Part	:PlasmaRingTrail.part
		Part	:PlasmaBoltGlow.part
		Part	:PlasmaBoltStreak.part
		Part	:PlasmaBoltTrail.part
		
		Part	:PlasmaBlastHead.part
		Part	:PlasmaBlastCore.part


		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	Time
	Time 	36

	Event
		Type	Local 
		At	p2

		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:PlasmaBlastFlash.part
		Part	:PlasmaBlastStar.part
		
		Part	:PlasmaFire.part
		
		LifeSpan	5
	End

	Event
		EName 	Prime1
		Type	Start
		At	p1
		Altpiv 1
		
		BHVR	:PlasmaBoltProjectile.bhvr
		bhvrOverride
			InitialVelocity		0.0 0.1 0.0
			InitialVelocityJitter	0.1 0.1 0.0
		End

		Part	:PlasmaRingTrail.part
		Part	:PlasmaBoltGlow.part
		Part	:PlasmaBoltStreak.part
		Part	:PlasmaBoltTrail.part
		
		Part	:PlasmaBlastHead.part
		Part	:PlasmaBlastCore.part


		Magnet	Target
		Lookat	Target

	End

End

Condition
	On 	Time
	Time 	43

	Event
		Type	Local 
		At	p2

		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:PlasmaBlastFlash.part
		Part	:PlasmaBlastStar.part
		
		Part	:PlasmaFire.part
		
		LifeSpan	5
	End

	Event
		EName 	Prime2
		Type	Start
		At	p1
		Altpiv 1
		
		BHVR	:PlasmaBoltProjectile.bhvr
		bhvrOverride
			InitialVelocity		0.3 0.0 0.0
			InitialVelocityJitter	0.1 0.1 0.0
		End

		Part	:PlasmaRingTrail.part
		Part	:PlasmaBoltGlow.part
		Part	:PlasmaBoltStreak.part
		Part	:PlasmaBoltTrail.part
		
		Part	:PlasmaBlastHead.part
		Part	:PlasmaBlastCore.part


		Magnet	Target
		Lookat	Target

	End

End

##################################


Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End


End

Condition
	On	Prime1Hit

	Event
		Ename	Prime1
		Type	Destroy
	End


End

Condition
	On	Prime2Hit

	Event
		Ename	Prime2
		Type	Destroy
	End


End

End				