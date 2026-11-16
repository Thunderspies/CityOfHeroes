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

Input  
	Inpname	HandL
End

LifeSpan	200

########################################################


Condition
	On 	Time
	Time 	25

	Event
		Type	start
		At	HandR
		Part1	:ChargedBolt06.part
		Part2	:ChargedBolt05.part
		Part3	:ChargedBolt01.part
		Part4	:ChargedBolt02.part
		POther Target	
	End		
	
End

Condition
	On 	Time
	Time 	28

	Event
		Type	start
		At	HandR
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
		At	HandR
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
		At	HandR
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
		At	HandR
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	42

	Event
		Type	start
		At	HandR
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
		At	HandR
		#Part2	:QuickBolt01a.part
		Part3	:ChargedBolt06.part
		Part4	:ChargedBolt05.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

######################################################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	start
		At	HandL
		Part1	:ChargedBolt06.part
		Part2	:ChargedBolt05.part
		Part3	:ChargedBolt01.part
		Part4	:ChargedBolt02.part
		POther Target	
	End		
	
End

Condition
	On 	Time
	Time 	28

	Event
		Type	start
		At	HandL
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	33

	Event
		Type	start
		At	HandL
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	36

	Event
		Type	start
		At	HandL
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	40

	Event
		Type	start
		At	HandL
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	43

	Event
		Type	start
		At	HandL
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
		At	HandL
		#Part2	:QuickBolt01a.part
		Part3	:ChargedBolt06.part
		Part4	:ChargedBolt05.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

#############################################################################
##Hands
############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		SOund	lightningbolt3 80 30 .76
		

	End

	Event
		EName 	Thingy1
		Type	local
		At	HandL
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

	Event
		EName 	ThingyB
		Type	local
		At	HandL
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		
	End
End

Condition
	On 	Time
	Time 	25
	Event
		EName 	BurstR
		Type	local
		At	HandR
		Part4	:ElectricitySparkBurst.part
		
		LifeSpan	20

	End

	Event
		EName 	BurstL
		Type	local
		At	HandL
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
	Time 	55
	Event
		
		EName	Thingy
		Type	destroy
		
		
	End

	Event
		
		EName	Thingy1
		Type	destroy
		
		
	End

	Event
		
		EName	ThingyA
		Type	destroy
		
		
	End

	Event
		
		EName	ThingyB
		Type	destroy
		
		
	End

	

End

End