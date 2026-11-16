#############################################################
## IceBoltAttack.fx
#############################################################

FxInfo
Lifespan 200

## HANDS ###########################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Ename   LeftHandMist
#		Type	Local
#		At	WepL
#		Part1	:snowburst.part
#		Part2	:ColdArmMist.part
#		LifeSpan 27
#	End
#
#	Event
#		Ename   RightHandMist
#		Type	Local
#		At	WepR
#		Part1	:snowburst.part
#		Part2	:ColdArmMist.part
#		Sound	Ice8 100.0 30.0 1.0
#		LifeSpan 27
#	End
#End

## ATTACK ###########################################################

#Condition
#	On 	Time
#	Time 	17
#
#	Event
#		EName   Flash1
#		Type	start
#		At	WepR
#		Part1	:IceBoltSnowSkatter.part
#		Lifespan 30
#	End
#End

## PROJECTILE ###########################################################

Condition
	On 	Time
	Time 	17

	Event
		EName 	Prime
		Type	Start
		At	WepR
		Sound Demon_Cast_01 100 100 .8
		Sound Demon_Cast_02 100 100 .8
		Sound Demon_Cast_03 100 100 .8
		Bhvr	behaviors\powers\coldcontrol\icebolt.bhvr
		Part	:IceBolt_Mist_Tail_Small.part
		Part	:IceBolt_Wind_Streak_Small.part
		Part	:IceBolt_Snow_Streak.part
		Part	:IceBolt_Mist_Tail_Core.part
		Part	:IceBolt_Snow_Trail.part
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Prime2
		Type	Start
		At	WepR
		Bhvr	behaviors\powers\coldcontrol\icebolt.bhvr
		BhvrOverride
			Alpha		255
			StartColor	180 240 255
			Scale		1.0 1.0 2.0
			ScaleRate	0 0 0
			EndScale	1.0 1.0 2.0
		End
		Geom	Tintable_FX_IceBlast
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

Condition
	On	Prime2Hit

	Event
		Ename	Prime2
		Type	Destroy
	End
End

#############################################################

End