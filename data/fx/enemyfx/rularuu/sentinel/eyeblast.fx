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
		BHVR	Behaviors/SentinelOffsetEye.bhvr

	End

	Event
		ENAME	Geoma
		Type	local
		At	Eye
		bhvr	behaviors/SentinelHoldProjectile.bhvr
		Geom	SentinelEnergyWave
		
		LifeSpan	30
	End

		
End

End			