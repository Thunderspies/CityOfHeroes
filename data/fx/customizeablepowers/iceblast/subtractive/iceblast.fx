#############################################################
## IceBlast.fx
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
		Part2	:ColdArmMist.part
		Part1	:snowburst.part
		LifeSpan 27
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	:snowburst.part
		Part2	:ColdArmMist.part
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
		Part1	:IceBoltSnowSkatter.part
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
		Bhvr	CustomizeablePowers\IceBlast\icebolt.bhvr
		BhvrOverride
			Alpha			150
			TintGeom		1
		End
		Geom	Tintable_Dark_FX_IceBolt
		Part1	:icebolttail1.part
		Part3	:IceBoltSnowtrail.part
		Part4	:IceBoltMistTrail.part
		Part5	:IceBoltIceDebris2.part
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