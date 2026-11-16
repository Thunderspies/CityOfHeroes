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
		Part1	CustomizeablePowers\IcyAssault\snowburst.part
		Part2	CustomizeablePowers\IcyAssault\ColdArmMist.part
		LifeSpan 100
	End

	Event
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IcyAssault\snowburst.part
		Part2	CustomizeablePowers\IcyAssault\ColdArmMist.part
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
		Part1	CustomizeablePowers\IcyAssault\Frost.part
		LifeSpan 110
	End

	Event
		Type	Local
		At	UarmR
		Part1	CustomizeablePowers\IcyAssault\Frost.part
		LifeSpan 110
	End

	Event
		Type	Local
		At	LarmL
		Part1	CustomizeablePowers\IcyAssault\Frost.part
		LifeSpan 110
	End

	Event
		Type	Local
		At	LarmR
		Part1	CustomizeablePowers\IcyAssault\Frost.part
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
		Part1	CustomizeablePowers\IcyAssault\IceChunksHands.part
		LifeSpan 40
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IcyAssault\IceChunksHands.part
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
		Bhvr	CustomizeablePowers\IcyAssault\icebolt2.bhvr
		Geom	FX_IceBolt
		Part1	CustomizeablePowers\IcyAssault\FreezeRayGlow.part
		Part2	CustomizeablePowers\IcyAssault\FreezeRayIceChuncks2.part
		Part3	CustomizeablePowers\IcyAssault\FreezeRayIceChuncks.part
		Part4	CustomizeablePowers\IcyAssault\BitterFreezeRayIceDebris.part
		Part5	CustomizeablePowers\IcyAssault\BitterFreezeRayIceDebris2.part
		Sound Ice9 100.0 100.0 .88
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	trail
		Type	Start
		At	mystic
		Bhvr	CustomizeablePowers\IcyAssault\iceblast.bhvr
		Part1	CustomizeablePowers\IcyAssault\FreezeRayMist.part
		Part2	CustomizeablePowers\IcyAssault\FreezeRaycrystalStreak.part
		Part3	CustomizeablePowers\IcyAssault\BitterIceboltFrost2.part
		Part4	CustomizeablePowers\IcyAssault\IceBoltSnowtrail2.part
		Part5	CustomizeablePowers\IcyAssault\FreezeRaycrystalStreak.part
		Part	CustomizeablePowers\IcyAssault\FreezeRaycrystalStreak.part
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