#########################################################
##	forceBubble
########################################################

FxInfo

LifeSpan	65

########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Waist
		Part	:Hamidon_Mito_Mez_Core.part
		Lifespan	65
	End		
End

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Waist
		Part	:Hamidon_Mito_Mez_Twinkles.part
		Lifespan	65
	End		
End

########################################################

Condition
	On 	Time
	Time 	18

	Event
		Type	local
		At	Waist
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part
		Part3	:ChargedBolt01.part
		Part4	:ChargedBolt02.part
		POther	T_Waist	
		Sound mitoblast1 70 70 .7
	End		
End

Condition
	On 	Time
	Time 	25

	Event
		Type	start
		At	Waist
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  T_Waist
		Sound heal3b 70 70 .66
	End
End

Condition
	On 	Time
	Time 	28

	Event
		Type	start
		At	Waist
		Part4	:ChargedBolt01.part
		Part5	:ChargedBolt02.part
		POther  T_Waist
		Sound heal3c 70 70 .66
	End
End

Condition
	On 	Time
	Time 	32

	Event
		Type	start
		At	Waist
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  T_Waist
	End
End

Condition
	On 	Time
	Time 	35

	Event
		Type	start
		At	Waist
		Part4	:ChargedBolt01.part
		Part5	:ChargedBolt02.part
		POther  T_Waist
	End
End

Condition
	On 	Time
	Time 	39

	Event
		Type	start
		At	Waist
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  T_Waist
	End
End

Condition
	On 	Time
	Time 	42

	Event
		Type	start
		At	Waist
		Part4	:ChargedBolt01.part
		Part5	:ChargedBolt02.part
		POther  T_Waist
	End
End

Condition
	On 	Time
	Time 	46
		
	Event
		Type	start
		At	Waist
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part
		POther  T_Waist
	End
End

Condition
	On 	Time
	Time 	49
		
	Event
		Type	start
		At	Waist
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part
		POther  T_Waist
	End
End

Condition
	On 	Time
	Time 	52
		
	Event
		Type	start
		At	Waist
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part
		POther  T_Waist
	End
End

Condition
	On 	Time
	Time 	56
		
	Event
		Type	start
		At	Waist
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part
		POther  T_Waist
	End
End

Condition
	On 	Time
	Time 	60
		
	Event
		Type	start
		At	Waist
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part
		Part3	:ChargedBolt01.part
		Part4	:ChargedBolt02.part
		POther  T_Waist
	End
End

########################################################

Condition
	On 	PrimeHit
	
	Event
		
		EName	Prime
		Type	destroy
	End
End

Condition
	On 	Time
	Time 	65

	Event
		
		EName	Thingy1
		Type	destroy
	End

	Event
		EName	ThingyB
		Type	destroy
	End
End

########################################################

End