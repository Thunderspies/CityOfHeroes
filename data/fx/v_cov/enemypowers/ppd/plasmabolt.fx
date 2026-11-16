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
		ENAME   P2
		Type	Local 
		At	LarmR	
		BhvrOverride
			PositionOffset	1.6 -.05 -.35
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
	Time 	30

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
		ENAME   p1
		Type	Local 
		At	p2

		Geom	centerdummy
	End

	Event
		EName 	Prime
		Type	Start
		At	p1
		Altpiv 1
		
		BHVR	:PlasmaBoltProjectile.bhvr

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

End				