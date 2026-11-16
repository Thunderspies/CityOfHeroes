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
		Part1	CustomizeablePowers\FireManipulation\FireRingFlames.part
		Part1	CustomizeablePowers\FireManipulation\Yellow3a.part
		Part3	CustomizeablePowers\FireManipulation\BlowGlowfire.part
		Part2	CustomizeablePowers\FireManipulation\fireringspark.part
		Part4	CustomizeablePowers\FireManipulation\FireBallSpark.part
		Sound Torch2_loop 50 50 .6
	End
End

Condition
	On 	Time
	Time 	18

	Event
		Type	posit
		At	root
		Part1	CustomizeablePowers\FireManipulation\YellowFlame02.part
	End
End

#############################################################

End