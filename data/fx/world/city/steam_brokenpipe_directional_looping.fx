#############################################################
## Steam_BrokenPipe_Directional_Looping.fx
#############################################################


FxInfo
#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Part	:Steam_BrokenPipe_Directional.part
		Part	:Steam_BrokenPipe_Directional_Highlights.part
		Part	:Steam_Pillar.part
		SoundNoRepeat 3
		Sound SteamBlast_01 35 35 .35
		Sound SteamBlast_02 35 35 .35
		Sound SteamBlast_03 35 35 .35
		Sound SteamBlast_04 35 35 .35
		Sound SteamBlast_05 35 35 .35
		Sound SteamBlast_06 35 35 .35
		Lifespan 10
	End
End

Condition
	On 	Cycle
	Time 	60
	Chance 0.5

	Event
		Type	Local
		At	Origin
		Part	:Steam_BrokenPipe_Directional.part
		Part	:Steam_BrokenPipe_Directional_Highlights.part
		Part	:Steam_Pillar.part
		SoundNoRepeat 3
		Sound SteamBlast_01 35 35 .35
		Sound SteamBlast_02 35 35 .35
		Sound SteamBlast_03 35 35 .35
		Sound SteamBlast_04 35 35 .35
		Sound SteamBlast_05 35 35 .35
		Sound SteamBlast_06 35 35 .35
		Lifespan 20
	End
End


#############################################################

End