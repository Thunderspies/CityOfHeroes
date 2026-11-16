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
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Sound electricalring_loop 40 40 .32
	End

	Event
		EName 	Prime
		Type	Local
		At	WepRight
		Altpiv	1
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part	ENEMYFX\Carnival_Of_Shadows\Ring_Electrical_Bomb\ElectricityOuterGlow.part
		Part	ENEMYFX\Carnival_Of_Shadows\Ring_Electrical_Bomb\Ring_Toss_Streaks.part
		#Part	ENEMYFX\Carnival_Of_Shadows\Ring_Electrical_Bomb\Ring_Sparks.part
		Part	ENEMYFX\Carnival_Of_Shadows\Ring_Electrical_Bomb\ElectricityControl01.part
		Part	ENEMYFX\Carnival_Of_Shadows\Ring_Electrical_Bomb\ElectricityControl02.part
		Part	ENEMYFX\Carnival_Of_Shadows\Ring_Electrical_Bomb\ElectricityInnerGlow.part
		Part	ENEMYFX\Carnival_Of_Shadows\Ring_Electrical_Bomb\ElectricityArch.part
		Part	ENEMYFX\Carnival_Of_Shadows\Ring_Electrical_Bomb\ElectricityRing.part
		Part	ENEMYFX\Carnival_Of_Shadows\Ring_Electrical_Bomb\ElectricityArchInvert.part
	End
End

End