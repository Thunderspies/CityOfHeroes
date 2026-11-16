#########################################################
##	
##

FxInfo

LifeSpan  400

##################################


Condition
	On	Time
	Time	48
	

	Event
		Type	Local
		At 	Origin
		Sound RobotHeavyLazerBurst2 100 100 .9
	End
End

Condition
	On 	Time
	Time 	58  #20

	Event
		Ename	Launch
		Type	Local 
		At	C_Head

		BHVR	Behaviors/GenericParticleFade.Bhvr
		BhvrOverride
			PositionOffset	0 -.25 -2.2
		End
		Part	:PlasmaBlastFlash.part
		Part	:PlasmaBlastStar.part
		
		Part	:PlasmaFire.part
		
		LifeSpan	5
	End

	Event
		EName 	Prime
		Type	Start
		At	Launch
		
		BHVR	:PlasmaBoltProjectile.bhvr
		BhvrOverride
			TrackRate	4
		End

		Part	:PlasmaRingTrail.part
		Part	:PlasmaBoltGlow.part
		Part	:PlasmaBoltStreak.part
		Part	:PlasmaBoltTrail.part
		
		Part	:PlasmaBlastHead.part
		Part	:PlasmaBlastCore.part

		Part	:PlasmaBlastHead.part
		Part	:PlasmaBlastCore.part
		
		Part	:PlasmaFire.part
		Part	:PlasmaHitSparks.part

		Magnet	Target
		Lookat	Target

	End

#	Event
#		Type	Local 
#		At	WepR
#
#		BHVR	Behaviors/GenericParticleFade.Bhvr
#		Part	:PlasmaBlastFlash.part
#		Part	:PlasmaBlastStar.part
#		
#		Part	:PlasmaFire.part
#		
#		LifeSpan	5
#	End
#
#	Event
#		EName 	Prime1
#		Type	Start
#		At	WepR
#		
#		BHVR	:PlasmaBoltProjectile.bhvr
#		BhvrOverride
#			TrackRate	4
#		End
#
#		Part	:PlasmaRingTrail.part
#		Part	:PlasmaBoltGlow.part
#		Part	:PlasmaBoltStreak.part
#		Part	:PlasmaBoltTrail.part
#		
#		Part	:PlasmaBlastHead.part
#		Part	:PlasmaBlastCore.part
#
#		Part	:PlasmaBlastHead.part
#		Part	:PlasmaBlastCore.part
#		
#		Part	:PlasmaFire.part
#		Part	:PlasmaHitSparks.part
#
#		Magnet	Target
#		Lookat	Target
#
#	End

End

##################################


Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Prime1
		Type	Destroy
	End

	Event
		Type	Start
		At	Target

		ChildFX	:PlasmaExplosion.fx

		LifeSpan	20
	End
	

End

End				