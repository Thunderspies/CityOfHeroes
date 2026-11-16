#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Torchfire
		Type	Local
		At	WepR
		Geom	GEO_WepR_Staff_07
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	TorchFire
		Altpiv   1
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		Part1	ENEMYFX\Carnival_Of_Shadows\Fire_Breath\TorchflameA.part
		Part2	ENEMYFX\Carnival_Of_Shadows\Fire_Breath\TorchflameB.part
		Part3	ENEMYFX\Carnival_Of_Shadows\Fire_Breath\TorchsparksA.part
		Sound Torch1_loop 22.0 22.0 1.0
	End
End

#############################################################

End