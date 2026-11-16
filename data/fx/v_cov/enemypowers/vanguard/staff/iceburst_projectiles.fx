#########################################################
## Staff of the M.A.G.I. - Ice Burst (PBAOE)
#########################################################

FxInfo

LifeSpan	200

## REVERSE PROJECTILE (From Target to Staff) ###############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	StaffEnd
		Type	Local 
		At	T_WepR
		BhvrOverride
			PositionOffset	0 0 2
		End
	End
End

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	Prime
		Type	Start 
		At	StaffEnd
		Bhvr	behaviors/powers/coldcontrol/icebolt.bhvr
		Geom	FX_IceBolt
		Part1	POWERS\ColdControl\icebolttail1.part
		Part2	POWERS\ColdControl\icebolttail1.part
		Part3	POWERS\ColdControl\IceBoltSnowtrail.part
		Part4	POWERS\ColdControl\IceBoltMistTrail.part
		Part5	POWERS\ColdControl\IceBoltIceDebris2.part
		Magnet	Chest
		LookAt	Chest
	End
End

## HIT FX ###############################################################

Condition
	On	PrimeHit
	
	Event
		EName	IceShards
		Type	Local
		At	Chest
		Part1	POWERS\ColdControl\IceBoltMist.part
		Part2	POWERS\ColdControl\ChillTouchSnow.part
		Part3   POWERS\ColdControl\ChillingTouchFlash.part
		lifespan	20
	End

	Event
		Ename	Prime
		Type	Destroy
	End
End

#################################################################

End