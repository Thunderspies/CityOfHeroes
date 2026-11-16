#########################################################
## Staff of the M.A.G.I. - Ice Beam
#########################################################

FxInfo

LifeSpan	200

## SOUND FX #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	fist
		Type	Local 
		At	WepR
		Sound Ice9 100.0 100.0 .75
	End
End

## CAST FX #######################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	StaffEnd
		Type	Local 
		At	WepR
		BhvrOverride
			PositionOffset	0 0 2
		End
	End
End

Condition
	On	Time
	Time	25

	Event	
		Ename   RightHandMist
		Type	Local
		At	StaffEnd
		Part1	POWERS\ColdControl\snowburst.part
		Part2	POWERS\ColdControl\ColdArmMist.part
		LifeSpan	37
	End
End

## PROJECTILE #######################################################

Condition
	On 	Time
	Time 	25

	Event
		EName 	Prime
		Type	Start 
		At	StaffEnd
		Bhvr	behaviors/powers/coldcontrol/iceblast.bhvr
		Geom	FX_IceBlast
		Magnet	Target
		LookAt	Target
		Part1	POWERS\ColdControl\icebolttail3.part
		Part2	POWERS\ColdControl\icebolttail2.part
		Part3	POWERS\ColdControl\IceBoltSnowtrail2.part
		Part4	POWERS\ColdControl\IceBoltMistTrail2.part
		Part5	POWERS\ColdControl\icebolttail.part
	End
End

## ON HIT ############################################################

Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End

	Event	
		Ename   IceShards
		Type	Posit
		At	target
		Part1	POWERS\ColdControl\IceBoltMist.part
		Part2	POWERS\ColdControl\ChillTouchSnow.part
		Part3   POWERS\ColdControl\ChillingTouchFlash.part

		LifeSpan	20
	End
End

#########################################################

End				