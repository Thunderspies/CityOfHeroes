#############################################################
## FireRing.fx
#############################################################

FxInfo

#############################################################

Input
	InpName	Hips
End

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	Ring
		Type	Posit
		At	Root
		Part1	powers/firecontrol/Yellow2.part
		Part2	powers/firecontrol/fireringspark.part
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
		Part1	powers/firecontrol/YellowFlame01.part
		Part1	powers/firecontrol/YellowFlameA.part
	End


End

####################################################################################

#Condition
#	On 	cycle
#	Time 	3
#
#	Event
#		Type	posit
#		At	ring
#
#		childfx	:BurningRingSplatChild2a.fx
#		LifeSpan	5
#	End
#
#End

#Condition
#	On 	cycle
#	Time 	3
#	chance	.8
#
#	Event
#		Type	posit
#		At	ring
#
#		childfx	:BurningRingSplatChild2a.fx
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
#		childfx	:BurningRingSplatChild2a.fx
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
#		childfx	:BurningRingSplatChild2a.fx
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
#		childfx	:BurningRingSplatChild2a.fx
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
#		childfx	:BurningRingSplatChild2a.fx
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
#		childfx	:BurningRingSplatChilda.fx
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
#		childfx	:BurningRingSplatChilda.fx
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
#		childfx	:BurningRingSplatChilda.fx
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
#		childfx	:BurningRingSplatChilda.fx
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
#		childfx	:BurningRingSplatChilda.fx
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
#		childfx	:BurningRingSplatChilda.fx
#		LifeSpan	5
#	End
#
#End

End