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

LifeSpan	250

########################################################


Condition
	On 	Time
	Time 	30

	Event
		Type	start
		At	HandR
		Part1	CustomizeablePowers\ElectricalBlast\ChargedBolt06.part
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt05.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt01.part
		Part4	CustomizeablePowers\ElectricalBlast\ChargedBolt02.part
		POther Target	
	End		
	
End

Condition
	On 	Time
	Time 	33

	Event
		Type	start
		At	HandR
		Part4	CustomizeablePowers\ElectricalBlast\ChargedBolt03.part
		Part5	CustomizeablePowers\ElectricalBlast\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	37

	Event
		Type	start
		At	HandR
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt01.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	40

	Event
		Type	start
		At	HandR
		Part4	CustomizeablePowers\ElectricalBlast\ChargedBolt03.part
		Part5	CustomizeablePowers\ElectricalBlast\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	44

	Event
		Type	start
		At	HandR
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt01.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	47

	Event
		Type	start
		At	HandR
		Part4	CustomizeablePowers\ElectricalBlast\ChargedBolt03.part
		Part5	CustomizeablePowers\ElectricalBlast\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	51
		
	Event

		Type	start
		At	HandR
		Part1	CustomizeablePowers\ElectricalBlast\ChargedBolt01.part
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt02.part
		
		POther  Target
	End

End

Condition
	On 	Time
	Time 	54
		
	Event

		Type	start
		At	HandR
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt03.part
		Part4	CustomizeablePowers\ElectricalBlast\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	57
	
	Event

		Type	start
		At	HandR
		Part1	CustomizeablePowers\ElectricalBlast\ChargedBolt01.part
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt02.part
		
		POther  Target
	End	

End

Condition
	On 	Time
	Time 	61

	Event

		Type	start
		At	HandR
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt03.part
		Part4	CustomizeablePowers\ElectricalBlast\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	64

	Event

		Type	start
		At	HandR
		Part1	CustomizeablePowers\ElectricalBlast\ChargedBolt01.part
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt02.part
		
		POther  Target
	End

End

Condition
	On 	Time
	Time 	67

	Event

		Type	start
		At	HandR
		Part1	CustomizeablePowers\ElectricalBlast\ChargedBolt01.part
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt06.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt05.part
		Part4	CustomizeablePowers\ElectricalBlast\ChargedBolt04.part
		POther  Target
	End

End

######################################################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	start
		At	HandL
		Part1	CustomizeablePowers\ElectricalBlast\ChargedBolt06.part
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt05.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt01.part
		Part4	CustomizeablePowers\ElectricalBlast\ChargedBolt02.part
		POther Target	
	End		
	
End

Condition
	On 	Time
	Time 	33

	Event
		Type	start
		At	HandL
		Part4	CustomizeablePowers\ElectricalBlast\ChargedBolt03.part
		Part5	CustomizeablePowers\ElectricalBlast\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	37

	Event
		Type	start
		At	HandL
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt01.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	40

	Event
		Type	start
		At	HandL
		Part4	CustomizeablePowers\ElectricalBlast\ChargedBolt03.part
		Part5	CustomizeablePowers\ElectricalBlast\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	44

	Event
		Type	start
		At	HandL
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt01.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	47

	Event
		Type	start
		At	HandL
		Part4	CustomizeablePowers\ElectricalBlast\ChargedBolt03.part
		Part5	CustomizeablePowers\ElectricalBlast\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	51
		
	Event

		Type	start
		At	HandL
		Part1	CustomizeablePowers\ElectricalBlast\ChargedBolt01.part
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt02.part
		
		POther  Target
	End

End

Condition
	On 	Time
	Time 	54
		
	Event

		Type	start
		At	HandL
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt03.part
		Part4	CustomizeablePowers\ElectricalBlast\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	57
	
	Event

		Type	start
		At	HandL
		Part1	CustomizeablePowers\ElectricalBlast\ChargedBolt01.part
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt02.part
		
		POther  Target
	End	

End

Condition
	On 	Time
	Time 	61

	Event

		Type	start
		At	HandL
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt03.part
		Part4	CustomizeablePowers\ElectricalBlast\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	64

	Event

		Type	start
		At	HandL
		Part1	CustomizeablePowers\ElectricalBlast\ChargedBolt01.part
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt02.part
		
		POther  Target
	End

End

Condition
	On 	Time
	Time 	67

	Event

		Type	start
		At	HandL
		Part1	CustomizeablePowers\ElectricalBlast\ChargedBolt01.part
		Part2	CustomizeablePowers\ElectricalBlast\ChargedBolt06.part
		Part3	CustomizeablePowers\ElectricalBlast\ChargedBolt05.part
		Part4	CustomizeablePowers\ElectricalBlast\ChargedBolt04.part
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
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlow.part
		Sound cagebolt2 80 80 .8
		
		

	End

	Event
		EName 	Thingy1
		Type	local
		At	HandL
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlow.part
		
	End


	Event
		EName 	ThingyA
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityRing.part
		
	End

	Event
		EName 	ThingyB
		Type	local
		At	HandL
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityRing.part
		
	End
End

Condition
	On 	Time
	Time 	25
	Event
		EName 	BurstR
		Type	local
		At	HandR
		Part4	CustomizeablePowers\ElectricalBlast\ElectricitySparkBurst.part
		SOund	lightningbolt2 80 30 .76
		LifeSpan	20

	End

	Event
		EName 	BurstL
		Type	local
		At	HandL
		Part4	CustomizeablePowers\ElectricalBlast\ElectricitySparkBurst.part
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
	Time 	71
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