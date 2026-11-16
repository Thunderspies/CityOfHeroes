#############################################################
## BitterFreezeRay.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	WepL
		Part1	:snowburst.part
		Part2	:ColdArmMist.part
		LifeSpan 100
	End

	Event
		Type	Local
		At	WepR
		Part1	:snowburst.part
		Part2	:ColdArmMist.part
		Sound Icebuild2 100.0 100.0 0.86
		LifeSpan 100
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UarmL
		Part1	:Frost.part
		LifeSpan 110
	End

	Event
		Type	Local
		At	UarmR
		Part1	:Frost.part
		LifeSpan 110
	End

	Event
		Type	Local
		At	LarmL
		Part1	:Frost.part
		LifeSpan 110
	End

	Event
		Type	Local
		At	LarmR
		Part1	:Frost.part
		LifeSpan 110
	End
End

##################################################################

Condition
	On	Time
	Time	68

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	:IceChunksHands.part
		LifeSpan 40
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	:IceChunksHands.part
		LifeSpan 40
	End
End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	69

	Event
		EName 	Prime
		Type	Start
		At	mystic
		Bhvr	behaviors\powers\coldcontrol\icebolt2.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part1	:FreezeRayGlow.part
		Part2	:FreezeRayIceChuncks2.part
		Part3	:FreezeRayIceChuncks.part
		Part4	:BitterFreezeRayIceDebris.part
		Part5	:BitterFreezeRayIceDebris2.part
		Geom	tintable_FX_IceBolt
		Sound Ice9 100.0 100.0 0.88
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	trail
		Type	Start
		At	mystic
		Bhvr	behaviors\powers\coldcontrol\iceblast.bhvr
		Part1	:FreezeRayMist.part
		Part2	:FreezeRaycrystalStreak.part
		Part3	:BitterIceboltFrost2.part
		Part4	:IceBoltSnowtrail2.part
		Part5	:FreezeRaycrystalStreak.part
		Magnet	Target
		LookAt	Target
	End
End

## ON-HIT ###########################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	trail
		Type	Destroy
	End
End

#############################################################

End