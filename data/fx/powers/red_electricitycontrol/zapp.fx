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

Input  
	Inpname	Mystic
End

Input  
	Inpname	HandR
End

LifeSpan	200

########################################################
Condition
	On 	Time
	Time 	5

	Event
		Type	start
		At	HandR
		Part1	:QuickBolt06.part
		Part2	:QuickBolt05.part
		Part3	:QuickBolt01.part
		Part4	:QuickBolt02.part
		POther Target	
	End		
	
End

Condition
	On 	Time
	Time 	8

	Event
		Type	start
		At	HandR
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	12

	Event
		Type	start
		At	HandR
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	15

	Event
		Type	start
		At	HandR
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	19

	Event
		Type	start
		At	HandR
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	22

	Event
		Type	start
		At	HandR
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	26
		
	Event

		Type	start
		At	HandR
		#Part2	:QuickBolt01a.part
		Part3	:QuickBolt06.part
		Part4	:QuickBolt05.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	30

	Event
		Type	start
		At	HandR
		Part1	:QuickBolt06.part
		Part2	:QuickBolt05.part
		Part3	:QuickBolt01.part
		Part4	:QuickBolt02.part
		POther Target	
	End		
	
End

Condition
	On 	Time
	Time 	33

	Event
		Type	start
		At	HandR
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	37

	Event
		Type	start
		At	HandR
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	40

	Event
		Type	start
		At	HandR
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	44

	Event
		Type	start
		At	HandR
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	47

	Event

		Type	start
		At	HandR
		Part2	:QuickBolt01.part
		Part3	:QuickBolt06.part
		Part4	:QuickBolt05.part
		Part5	:QuickBolt04.part
		POther  Target
	End

End

######################################################################################


#############################################################################
##Hands
############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		
		

	End

	Event
		EName 	ThingyA
		Type	local
		At	HandR
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		
	End

	
End

Condition
	On 	Time
	Time 	0
	Event
		EName 	BurstR
		Type	local
		At	HandR
		Part4	:ElectricitySparkBurst.part
		SOund	lightningbolt2 80 30 .76
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
	Time 	55
	Event
		
		EName	Thingy
		Type	destroy
		
		
	End

	Event
		
		EName	ThingyA
		Type	destroy
		
		
	End

End

End