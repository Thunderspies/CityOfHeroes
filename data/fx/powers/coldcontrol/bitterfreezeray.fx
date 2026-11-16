#############################################################
## BitterIceBolt.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	10

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	POWERS\ColdControl\snowburst.part
		Part2	POWERS\ColdControl\ColdArmMist.part
		Sound bittericethrow3 100.0 100.0 .85
		LifeSpan 37
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	POWERS\ColdControl\snowburst.part
		Part2	POWERS\ColdControl\ColdArmMist.part
		LifeSpan 37
	End
End

#############################################################

Condition
	On 	Time
	Time 	18

	Event
		Type	start
		At	WepL
		Part1	POWERS\ColdControl\IceBoltSnowSkatter.part
		LifeSpan 90
	End
End

#############################################################

Condition
	On 	Time
	Time 	17

	Event
		EName 	Prime
		Type	Start
		At	WepL
		Bhvr	behaviors\powers\coldcontrol\iceblast.bhvr
		Geom	FX_IceBlast
		Part1	POWERS\ColdControl\BitterIceboltFrost.part
		Part2	POWERS\ColdControl\FreezeRayGlowA.part
		Part3	POWERS\ColdControl\FreezeRayMistA.part
		Part4	POWERS\ColdControl\Bittericebolttail.part
		Part	POWERS\ColdControl\IceBoltSnowtrail3.part
		Part	POWERS\ColdControl\Bittericebolttail.part
		Part	POWERS\ColdControl\Bittericebolttail.part
		Magnet	Target
		LookAt	Target
	End
End

##############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End

#############################################################

End