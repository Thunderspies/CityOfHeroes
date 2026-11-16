#############################################################
## BurningRing.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	15


	Event
		EName 	Ring1
		Type	Posit
		At	Root
		Part	:FireRingFlames.part
		Part	:Yellow3a.part
		Part	:fireringspark.part
		Part	:FireBallSpark.part
		Sound Torch2_loop 50 50 .6

	End

End

Condition
	On 	Time
	Time 	18

	Event
		EName 	ring
		Type	posit
		At	root
		Part1	:YellowFlame02.part
	End


End




