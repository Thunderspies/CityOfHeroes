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
		Bhvr	behaviors\powers\coldcontrol\icebolt.bhvr
		Part	:IceBolt_Wind_Streak.part
		Part	:IceBolt_Snow_Streak.part
		Part	:IceBolt_Rings.part
		Part	:IceBolt_Mist_Trail.part
		Part	:IceBolt_Snow_Trail.part
		Part	:IceBolt_Mist_Tail_Small.part
		Magnet	Target
		LookAt	Target
	End

	Event
		EName 	Prime2
		Type	Start
		At	WepR
		Bhvr	behaviors\powers\coldcontrol\icebolt.bhvr
		BhvrOverride
			StartColor	180 230 255
		End
		Geom	Tintable_FX_IceBolt
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