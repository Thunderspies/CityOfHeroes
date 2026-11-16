#########################################################
##	FireRing
##
FxInfo

LifeSpan	40	

###############################################################################

Condition
	On	Time
	Time	30

	Event
		EName	Eye
		Type	Local
		At	Chest
		BHVR	Behaviors/SentinelOffsetEyeHold.bhvr

	End

	Event
		ENAME	Prime
		Type	start
		At	Eye
		bhvr	behaviors/SentinelProjectile.bhvr
		Geom	SentinelEnergyWave2
		
		Magnet	Target
		LookAt	Target
		LifeSpan	200
	End

End

Condition
	
	On	PrimeHit
	
	Event
		ENAME	Prime
		Type	Destroy	

	End


End

End			