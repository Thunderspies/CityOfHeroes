#############################################################
## FreezeRay.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	0

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
		Sound FreezeRay2 100.0 100.0 0.8
		LifeSpan 37
	End
End

#############################################################

Condition
	On	Time
	Time	8

	Event
		Ename   RightHandMist
		Type	Local
		At	WepL
		Part1	:IceChunksHands.part
		LifeSpan 15
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	:IceChunksHands.part
		LifeSpan 15
	End
End

#############################################################

Condition
	On 	Time
	Time 	18

	Event
		Type	start
		At	WepL
		Part1	:IceBoltSnowSkatter.part
		LifeSpan 60
	End

	Event
		Type	start
		At	WepR
		Part1	:IceBoltSnowSkatter.part
		LifeSpan 60
	End
End

#############################################################

Condition
	On 	Time
	Time 	22

	Event
		EName 	Prime
		Type	Start
		At	Mystic
		Bhvr	:iceblast.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part1	:FreezeRayGlowA.part
		Part2	:FreezeRayIceChuncks2A.part
		Part3	:FreezeRayIceChuncksA.part
		Part4	:FreezeRayIceDebris.part
		Part5	:FreezeRayIceDebris2.part
		Geom	tintable_FX_IceBlast
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	trail
		Type	Start
		At	Mystic
		Bhvr	:iceblast.bhvr
		Part1	:FreezeRayMistA.part
		Part2	:FreezeRaycrystalStreakA.part
		Part5	:IceBoltSnowtrail2.part
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