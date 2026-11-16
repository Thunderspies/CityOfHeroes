#########################################################
##	jack
##

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		EName	WepRight
		Type	local
		At	WepR
		Geom	GEO_WepL_Ringblade_03
		Sound glow9b_loop 40 40 .22
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End

	Event
		EName 	Prime
		Type	Local
		At	WepRight
		Altpiv	1
		Part	ENEMYFX\Carnival_Of_Shadows\Ring_Energy\Ring_Glow_Main.part
		#Part	ENEMYFX\Carnival_Of_Shadows\Ring_Energy\Ring_Glow_Main2.part
		#Part	ENEMYFX\Carnival_Of_Shadows\Ring_Energy\Ring_Glow_01.part
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

End