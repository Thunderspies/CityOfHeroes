#############################################################
## HeatLossBlast.fx
#############################################################

FxInfo

Lifespan 400

#############################################################

Condition
	On 	Time
	Time 	8

	Event
		EName 	fist
		Type	Local
		At	WepR
		Sound IceNova4 80.0 80.0 0.9
	End

End

#############################################################

Condition
	On	Time
	Time	10

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part	:snowburst.part
		Part	:ColdArmMist.part
		Lifespan 37
	End


	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part	:snowburst.part
		Part	:ColdArmMist.part
		Lifespan 37
	End

	Event
		Type	start
		At	WepR
		Part	:IceBoltSnowSkatter.part
		Lifespan 30
	End

	Event
		Type	start
		At	WepL
		Part	:IceBoltSnowSkatter.part
		Lifespan 30
	End
End

Condition
	On	Time
	Time	18


	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part	:IceChunksHands.part
		Lifespan 15
	End
End

#############################################################

Condition
	On 	Time
	Time 	27

	Event
		EName 	Prime
		Type	Start
		At	WepR
		Bhvr	:MainIceblastFast.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_IceBlast
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	trail
		Type	Start
		At	WepR
		Bhvr	:MainiceblastFast.bhvr
		Part	:FreezeRayMistA.part
		Part	:FreezeRaycrystalStreakA.part
		Part	:IceBoltSnowtrail2.part
		Part	:FreezeRayFrost.part
		Part	:FreezeRayFrost2.part
		Magnet	Target
		LookAt	Target
	End
End

#############################################################

Condition
	On 	Time
	Time 	33

	Event
		EName 	Prime1
		Type	Start
		At	WepL
		Bhvr	:MainIceblastFast.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_IceBlast
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	trail1
		Type	Start
		At	WepL
		Magnet	Target
		LookAt	Target
		Bhvr	:MainiceblastFast.bhvr
		Part	:FreezeRayMistA.part
		Part	:FreezeRaycrystalStreakA.part
		Part	:FreezeRaycrystalStreakA.part
		Part	:FreezeRaycrystalStreakA.part
		Part	:IceBoltSnowtrail2.part
		Part	:FreezeRayFrost.part
		Part	:FreezeRayFrost2.part
		Part	:FreezeRayFrost2.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	45

	Event
		EName 	Prime2
		Type	Start
		At	WepR
		Bhvr	:MainIceblastFast.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_IceBlast
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	trail2
		Type	Start
		At	WepR
		Magnet	Target
		LookAt	Target
		Bhvr	:MainiceblastFast.bhvr
		Part	:FreezeRayMistA.part
		Part	:FreezeRaycrystalStreakA.part
		Part	:FreezeRaycrystalStreakA.part
		Part	:FreezeRaycrystalStreakA.part
		Part	:IceBoltSnowtrail2.part
		Part	:FreezeRayFrost.part
		Part	:FreezeRayFrost2.part
		Part	:FreezeRayFrost2.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	50

	Event
		EName 	Prime3
		Type	Start
		At	WepL
		Bhvr	:MainIceblastFast.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_IceBlast
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	trail3
		Type	Start
		At	WepL
		Magnet	Target
		LookAt	Target
		Bhvr	:MainiceblastFast.bhvr
		Part	:FreezeRayMistA.part
		Part	:FreezeRaycrystalStreakA.part
		Part	:FreezeRaycrystalStreakA.part
		Part	:FreezeRaycrystalStreakA.part
		Part	:IceBoltSnowtrail2.part
		Part	:FreezeRayFrost.part
		Part	:FreezeRayFrost2.part
		Part	:FreezeRayFrost2.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	57

	Event
		EName 	Prime4
		Type	Start
		At	WepR
		Bhvr	:MainIceblastFast.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_IceBlast
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	trail4
		Type	Start
		At	WepR
		Magnet	Target
		LookAt	Target
		Bhvr	:MainiceblastFast.bhvr
		Part	:FreezeRayMistA.part
		Part	:FreezeRaycrystalStreakA.part
		Part	:FreezeRaycrystalStreakA.part
		Part	:FreezeRaycrystalStreakA.part
		Part	:IceBoltSnowtrail2.part
		Part	:FreezeRayFrost.part
		Part	:FreezeRayFrost2.part
		Part	:FreezeRayFrost2.part
	End
End


############################################################

Condition
	On	PrimeHit

	Event
		Ename   IceBlastRing
		Type	start
		At	T_chest
		Part	:AOEWindClouds.part
		Part	:AOEWindClouds2.part
		Part	:AOEsnowBurst.part
		Lifespan 20
	End

	Event
		Ename   IceShards
		Type	Posit
		At	target
		Part	:IceBoltMist.part
		Part	:ChillTouchSnow.part
		Lifespan 20
	End

	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	trail
		Type	Destroy
	End
End

Condition
	On	Prime1Hit

	Event
		Ename	Prime1
		Type	Destroy
	End

	Event
		Ename	trail1
		Type	Destroy
	End
End

Condition
	On	Prime2Hit

	Event
		Ename	Prime2
		Type	Destroy
	End

	Event
		Ename	trail2
		Type	Destroy
	End
End

Condition
	On	Prime3Hit

	Event
		Ename	Prime3
		Type	Destroy
	End

	Event
		Ename	trail3
		Type	Destroy
	End

	Event
		Ename   IceBlastRing
		Type	start
		At	T_chest
		Part	:AOEsnowBurst2.part
		Lifespan 20
	End
End

Condition
	On	Prime4Hit

	Event
		Ename	Prime4
		Type	Destroy
	End

	Event
		Ename	trail4
		Type	Destroy
	End
End

#############################################################

End