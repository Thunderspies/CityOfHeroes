#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

LifeSpan	200

########################################################


######################################################################################

Condition
	On 	Time
	Time 	18

	Event
		Type	start
		At	Origin
		Part1	:ChargedBolt06.part
		Part2	:ChargedBolt05.part
		Part3	:ChargedBolt01.part
		Part4	:ChargedBolt02.part
		POther Target	
	End		
	
End

Condition
	On 	Time
	Time 	25

	Event
		Type	start
		At	Origin
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	28

	Event
		Type	start
		At	Origin
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	32

	Event
		Type	start
		At	Origin
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	35

	Event
		Type	start
		At	Origin
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	39

	Event
		Type	start
		At	Origin
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	42

	Event
		Type	start
		At	Origin
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	46
		
	Event

		Type	start
		At	Origin
		
		Part1	:QuickBolt06.part
		Part2	:QuickBolt05.part
		
		POther  Target
	End

End

Condition
	On 	Time
	Time 	49
		
	Event

		Type	start
		At	Origin
		
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part
		
		POther  Target
	End

End

Condition
	On 	Time
	Time 	52
		
	Event

		Type	start
		At	Origin
		
		Part1	:QuickBolt03.part
		Part2	:QuickBolt04.part
		
		POther  Target
	End

End

Condition
	On 	Time
	Time 	56
		
	Event

		Type	start
		At	Origin
		
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part
		
		POther  Target
	End

End

Condition
	On 	Time
	Time 	60
		
	Event

		Type	start
		At	Origin
		
		Part1	:QuickBolt06.part
		Part2	:ChargedBolt05.part
		Part3	:QuickBolt01.part
		Part4	:ChargedBolt02.part
		
		POther  Target
	End

End

######################################################################################

#############################################################################
##Hands
############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	ThingyB
		Type	local
		At	origin
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArchField.part
		Part4	:ElectricityArchField2.part
		Part5	:FieldArchs1.part
		Part3	:ElectricityRingField.part
		
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName 	BurstL
		Type	local
		At	origin
		Part4	:ElectricitySparkBurst.part
		LifeSpan	20

	End

End

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
		
		EName	ThingyB
		Type	destroy
		
		
	End

	

End

End