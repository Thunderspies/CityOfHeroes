#############################################################
## BitterIceBolt.fx
#############################################################

FxInfo

LifeSpan 200

## HANDS ###########################################################

Condition
	On	Time
	Time	10

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	:snowburst.part
		Part2	:ColdArmMist.part
		Sound bittericethrow3 100.0 100.0 .85
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
	Time 	18

	Event
		Type	start
		At	WepL
		Part1	:IceBoltSnowSkatter.part
		LifeSpan 90
	End
End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	17

	Event
		EName 	Prime
		Type	Start
		At	WepL
		Bhvr	behaviors\powers\coldcontrol\iceblast.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	tintable_FX_IceBlast
		Part	:BitterIceboltFrost.part
		Part	:FreezeRayGlowA.part
		Part	:FreezeRayMistA.part
		Part	:Bittericebolttail.part
		Part	:IceBoltSnowtrail3.part
		Magnet	Target
		LookAt	Target
	End
End

## ON-HIT ############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End

#############################################################

End