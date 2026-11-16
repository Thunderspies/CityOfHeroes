#############################################################
## Ice Blast
#############################################################

FxInfo

LifeSpan 200

## SOUND ###########################################################

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

## HANDS ###########################################################

Condition
	On	Time
	Time	5

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	:snowburst.part
		LifeSpan	37
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	:snowburst.part
		LifeSpan	37
	End

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part2	:ColdArmMist.part
		LifeSpan	37
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part2	:ColdArmMist.part
		LifeSpan	37
	End
End

Condition
	On 	Time
	Time 	19

	Event
		EName   Flash
		Type	local
		At	WepR
		Part1	:IceBoltHandFlash.part
		Part2	:IceBoltHandFlash2.part
		Lifespan	5
	End
End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	19

	Event
		EName 	Prime
		Type	Start
		At	WepR
		Bhvr	behaviors\powers\coldcontrol\iceblast.bhvr
		Geom	FX_IceBlast
		Part1	:icebolttail3.part
		Part2	:icebolttail2.part
		Part3	:IceBoltSnowtrail2.part
		Part4	:IceBoltMistTrail2.part
		Part5	:icebolttail.part
		Magnet	Target
		LookAt	Target
	End
End

## HIT ###########################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

#	Event
#		Ename   IceShards
#		Type	Posit
#		At	target
#		Part1	:IceBoltMist.part
#		Part2	:ChillTouchSnow.part
#		Part3   :ChillingTouchFlash.part
#		LifeSpan	20
#	End
End

#############################################################

End