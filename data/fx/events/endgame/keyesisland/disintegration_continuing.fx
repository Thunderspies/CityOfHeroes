#############################################################
## Disintegration_Continuing.fx
#############################################################

FxInfo
Flags DontSuppress

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On 	Time
	Time	0

	Event
		Type	PositOnly
		At	Chest
		Bhvr 	:ForceBubble_Disintegration.bhvr
		BhvrOverride
			Alpha		255
			StartColor	100 255 10
			PyrRotateJitter	180.0 180.0 0.0
			FadeInLength	20
			FadeOutLength	20
			Spin 		10 10 0
		End
		Geom	Disintegration_Bubble
	End

	Event
		Type	PositOnly
		At	Chest
		Bhvr 	:ForceBubble_Disintegration.bhvr
		BhvrOverride
			Alpha		255
			StartColor	50 255 10
			PyrRotateJitter	-180.0 -180.0 0.0
			FadeInLength	20
			FadeOutLength	20
			Spin 		10 10 0
		End
		Part	:Disintegration_Globs.part
		Part	:Disintegration_Border.part
		Geom	Disintegration_Bubble
	End
End

#########################################################

End