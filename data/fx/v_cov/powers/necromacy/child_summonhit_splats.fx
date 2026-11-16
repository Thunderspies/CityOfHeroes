#############################################################
## SummonHit.fx
#############################################################

FxInfo

#############################################################

Condition
	On	cycle
	Time	5
	chance	0.9

	Event
		ENAME	BaseTrail
		Type	start
		At	Origin
		BHVR	POWERS\DarknessControl\DarktendrilInDark2.bhvr
		Splat	DarknessSplat.tga
		LifeSpan 10
	End

	Event
		ENAME	BaseTrail
		Type	start
		At	Origin
		BHVR	POWERS\DarknessControl\DarktendrilA.bhvr
		Splat	DarknessSplat.tga
		LifeSpan 10
	End

	Event
		ENAME	BaseTrail1
		Type	start
		At	Origin
		BHVR	POWERS\DarknessControl\DarktendrilInA.bhvr
		Splat	DarknessSplat2.tga
		LifeSpan 10
	End

	Event
		ENAME	BaseTrail1
		Type	start
		At	Origin
		BHVR	POWERS\DarknessControl\DarktendrilInDark.bhvr
		Splat	DarknessSplat2.tga
		LifeSpan 10
	End
End

#############################################################

End