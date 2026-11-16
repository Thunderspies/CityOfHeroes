#############################################################
## FireRing.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	Ring
		Type	Posit
		At	Root
		Part	:Yellow2.part
		Part	:fireringspark.part
		Sound Torch2_loop 50 50 .5

	End
End


Condition
	On 	Time
	Time 	23

	Event
		EName 	ring1
		Type	Posit
		At	Root
		Part	:YellowFlame01.part
		Part	:YellowFlameA.part
	End


End