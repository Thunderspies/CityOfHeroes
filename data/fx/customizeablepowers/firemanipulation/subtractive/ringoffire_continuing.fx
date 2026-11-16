#############################################################
## RingOfFire_Continuing.FX
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
		Type	Posit
		At	Root
		Part1	:FireRingFlames.part
		Part1	:Yellow3a.part
		Part3	:BlowGlowfire.part
		Part2	:fireringspark.part
		Part4	:FireBallSpark.part
		Sound Torch2_loop 50 50 .6
	End
End

Condition
	On 	Time
	Time 	18

	Event
		Type	posit
		At	root
		Part1	:YellowFlame02.part
	End
End

#############################################################

End