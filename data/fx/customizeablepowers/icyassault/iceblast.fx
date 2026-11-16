#############################################################
## Ice Bolt
#############################################################

FxInfo

Lifespan 200

## HANDS ###########################################################

Condition
	On	Time
	Time	0

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\IcyAssault\snowburst.part
		Part2	CustomizeablePowers\IcyAssault\ColdArmMist.part
		LifeSpan 27
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IcyAssault\snowburst.part
		Part2	CustomizeablePowers\IcyAssault\ColdArmMist.part
		Sound Ice8 100.0 100.0 1.0
		LifeSpan 27
	End
End

## ATTACK ###########################################################

Condition
	On 	Time
	Time 	17

	Event
		EName   Flash1
		Type	start
		At	WepR
		Part1	CustomizeablePowers\IcyAssault\IceBoltSnowSkatter.part
		Lifespan 30
	End
End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	17

	Event
		EName 	Prime
		Type	Start
		At	WepR
		Bhvr	CustomizeablePowers\IcyAssault\icebolt.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_IceBolt
		Part1	CustomizeablePowers\IcyAssault\icebolttail1.part
		Part3	CustomizeablePowers\IcyAssault\IceBoltSnowtrail.part
		Part4	CustomizeablePowers\IcyAssault\IceBoltMistTrail.part
		Part5	CustomizeablePowers\IcyAssault\IceBoltIceDebris2.part
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