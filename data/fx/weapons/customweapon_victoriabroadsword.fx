#############################################################
## CustomWeapon_Broadsword.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		SetState RIGHTHAND
		SoundNoRepeat 2
		Sound WWV_DrawBlades_01 100 100 .8
		Sound WWV_DrawBlades_02 100 100 .8
		Sound WWV_DrawBlades_03 100 100 .8
		Sound WWV_DrawBlades_04 100 100 .8
		Sound WWV_DrawBlades_05 100 100 .8
	End
End

#############################################################

End