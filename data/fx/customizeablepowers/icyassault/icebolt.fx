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
		Part1	CustomizeablePowers\IcyAssault\snowburst.part
		LifeSpan	37
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IcyAssault\snowburst.part
		LifeSpan	37
	End

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part2	CustomizeablePowers\IcyAssault\ColdArmMist.part
		LifeSpan	37
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part2	CustomizeablePowers\IcyAssault\ColdArmMist.part
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
		Part1	CustomizeablePowers\IcyAssault\IceBoltHandFlash.part
		Part2	CustomizeablePowers\IcyAssault\IceBoltHandFlash2.part
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
		Bhvr	CustomizeablePowers\IcyAssault\iceblast.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_IceBlast
		Part1	CustomizeablePowers\IcyAssault\icebolttail3.part
		Part2	CustomizeablePowers\IcyAssault\icebolttail2.part
		Part3	CustomizeablePowers\IcyAssault\IceBoltSnowtrail2.part
		Part4	CustomizeablePowers\IcyAssault\IceBoltMistTrail2.part
		Part5	CustomizeablePowers\IcyAssault\icebolttail.part
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
End

#############################################################

End