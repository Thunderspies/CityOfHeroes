#############################################################
## Malignant_Continuing.fx
#############################################################

FxInfo
Flags DontSuppress

Lifespan 30

#########################################################
####################### AUDIO ###########################



#########################################################


Condition
	On 	Time
	Time	0

	Event
		Type	PositOnly
		At	Chest
		Bhvr 	Events\EndGame\KeyesIsland\ForceBubble_Disintegration.bhvr
		BhvrOverride
			Alpha		255
			StartColor	100 255 10
			PyrRotateJitter	-180.0 -180.0 0.0
			FadeInLength	20
			FadeOutLength	20
			Spin 		10 10 0
		End
		Part	:Malignant_Globs.part
		Part	:Malignant_Border.part
	End
End

#########################################################

End