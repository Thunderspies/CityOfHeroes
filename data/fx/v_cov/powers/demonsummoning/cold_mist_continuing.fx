#############################################################
## Cold_Mist_Continuing.fx
#############################################################

FXInfo
ClampMinScale 0.75 0.75 0.75
ClampMaxScale 1.25 1.25 1.25

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Sound Cold_loop 50.0 50.0 0.42
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 110
	End

	Event
		Type	Local
		At	Chest
		Part1	:BlockOfIce_Mist.part
		Part2	:BlockOfIce_Snow.part
	End
End

#########################################################

End