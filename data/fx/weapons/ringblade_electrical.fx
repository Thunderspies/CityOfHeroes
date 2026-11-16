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
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Geom	GEO_WepL_Ringblade_03
		Sound electricalring_loop 40 40 .32
	End

	Event
		EName 	Prime
		Type	Local
		At	WepRight
		Altpiv	1
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part	ENEMYFX\Carnival_Of_Shadows\Ring_Electrical\ElectricityOuterGlow.part
		Part	ENEMYFX\Carnival_Of_Shadows\Ring_Electrical\Ring_Toss_Streaks.part
		#Part	ENEMYFX\Carnival_Of_Shadows\Ring_Electrical\Ring_Sparks.part
		Part	ENEMYFX\Carnival_Of_Shadows\Ring_Electrical\ElectricityControl01Left.part
		Part	ENEMYFX\Carnival_Of_Shadows\Ring_Electrical\ElectricityControl02Left.part
		Part	ENEMYFX\Carnival_Of_Shadows\Ring_Electrical\ElectricityInnerGlowLeft.part
		Part	ENEMYFX\Carnival_Of_Shadows\Ring_Electrical\ElectricityArchLeft.part
		Part	ENEMYFX\Carnival_Of_Shadows\Ring_Electrical\ElectricityRingLeft.part
		Part	ENEMYFX\Carnival_Of_Shadows\Ring_Electrical\ElectricityArchInvertLeft.part
	End
End

End