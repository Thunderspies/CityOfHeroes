#############################################################
## DuplicatingGift.fx
#############################################################

FxInfo
	LifeSpan 40
###########################################################

Condition
	On	Time
	Time	15

	Event
		At WepR
		Type Start
		EName Gift_Top
		Bhvr CustomizeablePowers\EnergyBlast\PushProjectile.bhvr
		BhvrOverride
			InitialVelocity	0.0 1 0.0
			Gravity		0.04
			TrackRate	0.5
			PositionOffset 	0 -0.5 0
			PyrRotate	180 0 0
			Scale		1.4 1.4 1.4
		End
		Geom GEO_HolidayGift2_Bottom
		LookAt	Target
		Magnet	Target
		Lifespan 15
	End

	Event
		At WepR
		Type Start
		EName Gift_Top
		Bhvr CustomizeablePowers\EnergyBlast\PushProjectile.bhvr
		BhvrOverride
			InitialVelocity	0.0 1 0.0
			Gravity		0.04
			TrackRate	0.5
			PositionOffset 	-0.1 0.1 -0.1
			PyrRotate	180 0 0
			Scale		1.4 1.4 1.4
		End
		Geom GEO_HolidayGift2_Top
		LookAt	Target
		Magnet	Target
		Lifespan 15
	End
End
##############################################################
End