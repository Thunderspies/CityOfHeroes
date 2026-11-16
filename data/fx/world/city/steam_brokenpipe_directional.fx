#############################################################
## Steam_BrokenPipe_Directional.fx
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
		Part1	:Steam_BrokenPipe_Directional.part
		Part	:Steam_BrokenPipe_Directional_Highlights.part
		Part2	:Steam_Pillar.part
		Sound SteamBlast1_Loop 35 35 .5
	End
End

#############################################################

End