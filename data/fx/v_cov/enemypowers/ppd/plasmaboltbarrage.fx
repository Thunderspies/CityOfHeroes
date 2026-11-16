#########################################################
##	
##

FxInfo

LifeSpan  400

##################################


#Condition
#	On	Time
#	Time	10
#
#	Event	
#		Type	Local
#		At	origin
#		Sound	plamsa4 90 70 .9
#	End
#End


Condition
	On	Time
	Time	10
	

	
	Event
		ENAME   gunL
		Type	Local 
		At	LarmL
		BhvrOverride
			PositionOffset	-2.5 -.05 -.35
		End
	End

	Event
		ENAME   gunR
		Type	Local 
		At	LarmR	
		BhvrOverride
			PositionOffset	2.5 -.05 -.35
		End
		
	End
End

Condition
	On 	Time
	Time 	38


	Event
		Type	Local 
		At	gunR

		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:PlasmaBlastFlash.part
		Part	:PlasmaBlastStar.part
		
		Part	:PlasmaFire.part
		Sound pulserifle1 90 90 .6
		LifeSpan	5
	End

	Event
		EName 	Prime
		Type	Start
		At	gunR
		
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
	Time 	43

	Event
		Type	Local 
		At	gunL

		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:PlasmaBlastFlash.part
		Part	:PlasmaBlastStar.part
		
		Part	:PlasmaFire.part
		Sound pulserifle1 90 90 .6
		LifeSpan	5
	End

	Event
		EName 	Prime1
		Type	Start
		At	gunL
		
		BHVR	:PlasmaBoltProjectile.bhvr
		bhvrOverride
			InitialVelocity		-0.1 0.1 0.0
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
	Time 	48

	Event
		Type	Local 
		At	gunR

		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:PlasmaBlastFlash.part
		Part	:PlasmaBlastStar.part
		Sound pulserifle2 90 90 .65
		Part	:PlasmaFire.part
		
		LifeSpan	5
	End

	Event
		EName 	Prime2
		Type	Start
		At	gunR
		
		BHVR	:PlasmaBoltProjectile.bhvr
		bhvrOverride
			InitialVelocity		0.1 0.1 0.0
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
	Time 	53

	Event
		Type	Local 
		At	gunL

		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:PlasmaBlastFlash.part
		Part	:PlasmaBlastStar.part
		Sound pulserifle1 90 90 .6
		Part	:PlasmaFire.part
		
		LifeSpan	5
	End

	Event
		EName 	Prime3
		Type	Start
		At	gunL
		
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

Condition
	On 	Time
	Time 	58

	Event
		Type	Local 
		At	gunR

		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:PlasmaBlastFlash.part
		Part	:PlasmaBlastStar.part
		Sound pulserifle2 90 90 .64
		Part	:PlasmaFire.part
		
		LifeSpan	5
	End

	Event
		EName 	Prime4
		Type	Start
		At	gunR
		
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

Condition
	On 	Time
	Time 	63

	Event
		Type	Local 
		At	gunL

		BHVR	Behaviors/GenericParticleFade.Bhvr
		Part	:PlasmaBlastFlash.part
		Part	:PlasmaBlastStar.part
		Sound pulserifle3 90 90 .8
		Part	:PlasmaFire.part
		
		LifeSpan	5
	End

	Event
		EName 	Prime5
		Type	Start
		At	gunL
		
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

Condition
	On	Prime3Hit

	Event
		Ename	Prime3
		Type	Destroy
	End


End

Condition
	On	Prime4Hit

	Event
		Ename	Prime4
		Type	Destroy
	End


End

Condition
	On	Prime5Hit

	Event
		Ename	Prime5
		Type	Destroy
	End


End


End				