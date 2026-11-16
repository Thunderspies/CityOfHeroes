#############################################################
## FX_Bat_01.fx
#############################################################

FxInfo
#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Root
		Bhvr behaviors\Generic_FadeIn-FadeOut.bhvr
		BhvrOverride
			PositionOffset 0 -0.5 0
			Scale	0.1 0.1 0.1
			#PyrRotate 0 180 0
		End
		Anim	FX_Fish_Leaping_01
	End
End


#############################################################
End