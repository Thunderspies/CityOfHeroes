#############################################################
## BurningRing.fx
#############################################################

FxInfo

#############################################################

Input
	InpName	Root
End

Input
	InpName	Hips
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName 	Fire
		Type	Posit
		At	Hips
		#Part1	:BurningRingSteam.part
		#Part2	:BurningRingFlame2.part
		Sound Torch2_loop 50 50 .6

	End

	Event
		EName 	Ring1
		Type	Posit
		At	Root
		Part1	:FireRingFlames.part
		Part1	powers/firecontrol/Yellow3a.part
		Part3	:BlowGlowfire.part

		Part2	powers/firecontrol/fireringspark.part
		Part4	:FireBallSpark.part
	End

End

Condition
	On 	Time
	Time 	18

	Event
		EName 	ring
		Type	posit
		At	root
		Part1	powers/firecontrol/YellowFlame02.part
	End


End

####################################################################

#Condition
#	On 	cycle
#	Time 	3
#
#	Event
#		Type	posit
#		At	ring
#
#		childfx	:BurningRingSplatChild2.Fx
#		LifeSpan	5
#	End
#
#End
#
#Condition
#	On 	cycle
#	Time 	2
#	chance	.7
#
#	Event
#		Type	posit
#		At	ring
#
#		childfx	:BurningRingSplatChild2.Fx
#		LifeSpan	5
#	End
#
#End
#
#Condition
#	On 	cycle
#	Time 	3
#	chance	.6
#
#	Event
#		Type	posit
#		At	ring
#
#		childfx	:BurningRingSplatChild2.Fx
#		LifeSpan	5
#	End
#
#End
#
#Condition
#	On 	cycle
#	Time 	4
#	chance	.8
#
#	Event
#		Type	posit
#		At	ring
#
#		childfx	:BurningRingSplatChild2.Fx
#		LifeSpan	5
#	End
#
#End
#
#Condition
#	On 	cycle
#	Time 	3
#	chance	.5
#
#	Event
#		Type	posit
#		At	ring
#
#		childfx	:BurningRingSplatChild2.Fx
#		LifeSpan	5
#	End
#
#End
#####################################################################
#
#Condition
#	On 	cycle
#	Time 	3
#
#	Event
#		Type	posit
#		At	ring
#
#		childfx	:BurningRingSplatChild.Fx
#		LifeSpan	5
#	End
#
#End
#
#Condition
#	On 	cycle
#	Time 	2
#	chance	.8
#
#	Event
#		Type	posit
#		At	ring
#
#		childfx	:BurningRingSplatChild.Fx
#		LifeSpan	5
#	End
#
#End
#
#Condition
#	On 	cycle
#	Time 	2
#	chance	.7
#
#	Event
#		Type	posit
#		At	ring
#
#		childfx	:BurningRingSplatChild.Fx
#		LifeSpan	5
#	End
#
#End
#
#Condition
#	On 	cycle
#	Time 	2
#	chance	.5
#
#	Event
#		Type	posit
#		At	ring
#
#		childfx	:BurningRingSplatChild.Fx
#		LifeSpan	5
#	End
#
#End
#
#Condition
#	On 	cycle
#	Time 	3
#	chance	.5
#
#	Event
#		Type	posit
#		At	ring
#
#		childfx	:BurningRingSplatChild.Fx
#		LifeSpan	5
#	End
#
#End
#
#Condition
#	On 	cycle
#	Time 	2
#	chance	.9
#
#	Event
#		Type	posit
#		At	ring
#
#		childfx	:BurningRingSplatChild.Fx
#		LifeSpan	5
#	End
#
#End


End