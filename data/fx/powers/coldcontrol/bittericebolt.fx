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
		Part1	POWERS\ColdControl\snowburst.part
		Part2	POWERS\ColdControl\ColdArmMist.part
		LifeSpan 100
	End

	Event
		Type	Local
		At	WepR
		Part1	POWERS\ColdControl\snowburst.part
		Part2	POWERS\ColdControl\ColdArmMist.part
		Sound Icebuild2 100.0 100.0 .86
		LifeSpan 100
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UarmL
		Part1	POWERS\ColdControl\Frost.part
		LifeSpan 110
	End

	Event
		Type	Local
		At	UarmR
		Part1	POWERS\ColdControl\Frost.part
		LifeSpan 110
	End

	Event
		Type	Local
		At	LarmL
		Part1	POWERS\ColdControl\Frost.part
		LifeSpan 110
	End

	Event
		Type	Local
		At	LarmR
		Part1	POWERS\ColdControl\Frost.part
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
		Part1	POWERS\ColdControl\IceChunksHands.part
		LifeSpan 40
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	POWERS\ColdControl\IceChunksHands.part
		LifeSpan 40
	End
End

#############################################################

Condition
	On 	Time
	Time 	69	#17

	Event
		EName 	Prime
		Type	Start
		At	mystic
		Bhvr	behaviors\powers\coldcontrol\icebolt2.bhvr
		Geom	FX_IceBolt
		Part1	POWERS\ColdControl\FreezeRayGlow.part
		Part2	POWERS\ColdControl\FreezeRayIceChuncks2.part
		Part3	POWERS\ColdControl\FreezeRayIceChuncks.part
		Part4	POWERS\ColdControl\BitterFreezeRayIceDebris.part
		Part5	POWERS\ColdControl\BitterFreezeRayIceDebris2.part
		Sound Ice9 100.0 100.0 .88
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	trail
		Type	Start
		At	mystic
		Bhvr	behaviors\powers\coldcontrol\iceblast.bhvr
		Part1	POWERS\ColdControl\FreezeRayMist.part
		Part2	POWERS\ColdControl\FreezeRaycrystalStreak.part
		Part3	POWERS\ColdControl\BitterIceboltFrost2.part
		Part4	POWERS\ColdControl\IceBoltSnowtrail2.part
		Part5	POWERS\ColdControl\FreezeRaycrystalStreak.part
		Part	POWERS\ColdControl\FreezeRaycrystalStreak.part
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

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