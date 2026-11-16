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
		Sound Ice9 100.0 100.0 0.75
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
		Part2	:ColdArmMist.part
		LifeSpan 37
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	:snowburst.part
		Part2	:ColdArmMist.part
		LifeSpan 37
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
		Lifespan 5
	End
End

### PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	19

	Event
		EName	Core
		Type	Local
		At	Chest
		Geom	PsionicRigging
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
	End

	Event
		EName 	Prime
		Type	Start
		At	Core
		Bhvr	:iceblast.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part5	:icebolttail.part
		Part2	:icebolttail2.part
		Part3	:IceBoltSnowtrail2.part
		Part4	:IceBoltMistTrail2.part
		Geom	tintable_FX_IceBlast
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