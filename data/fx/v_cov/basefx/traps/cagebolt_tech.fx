#########################################################
##	forceBubble
########################################################
FxInfo

LifeSpan	250

########################################################


Condition
	On 	Time
	Time 	0

	Event
		Ename	Locator
		Type	start
		At	Root
		BhvrOverride
			PositionOffset 0 -3 0
		End
		
	End		

End

Condition
	On 	Time
	Time 	30

	Event
		Type	start
		At	Locator
		Part1	\POWERS\ElectricityControl\ChargedBolt06.part
		Part2	\POWERS\ElectricityControl\ChargedBolt05.part
		Part3	\POWERS\ElectricityControl\ChargedBolt01.part
		Part4	\POWERS\ElectricityControl\ChargedBolt02.part
		POther Target	
	End		
	
End

Condition
	On 	Time
	Time 	33

	Event
		Type	start
		At	Locator
		Part4	\POWERS\ElectricityControl\ChargedBolt03.part
		Part5	\POWERS\ElectricityControl\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	37

	Event
		Type	start
		At	Locator
		Part2	\POWERS\ElectricityControl\ChargedBolt01.part
		Part3	\POWERS\ElectricityControl\ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	40

	Event
		Type	start
		At	Locator
		Part4	\POWERS\ElectricityControl\ChargedBolt03.part
		Part5	\POWERS\ElectricityControl\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	44

	Event
		Type	start
		At	Locator
		Part2	\POWERS\ElectricityControl\ChargedBolt01.part
		Part3	\POWERS\ElectricityControl\ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	47

	Event
		Type	start
		At	Locator
		Part4	\POWERS\ElectricityControl\ChargedBolt03.part
		Part5	\POWERS\ElectricityControl\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	51
		
	Event

		Type	start
		At	Locator
		Part1	\POWERS\ElectricityControl\ChargedBolt01.part
		Part2	\POWERS\ElectricityControl\ChargedBolt02.part
		
		POther  Target
	End

End

Condition
	On 	Time
	Time 	54
		
	Event

		Type	start
		At	Locator
		Part3	\POWERS\ElectricityControl\ChargedBolt03.part
		Part4	\POWERS\ElectricityControl\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	57
	
	Event

		Type	start
		At	Locator
		Part1	\POWERS\ElectricityControl\ChargedBolt01.part
		Part2	\POWERS\ElectricityControl\ChargedBolt02.part
		
		POther  Target
	End	

End

Condition
	On 	Time
	Time 	61

	Event

		Type	start
		At	Locator
		Part3	\POWERS\ElectricityControl\ChargedBolt03.part
		Part4	\POWERS\ElectricityControl\ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	64

	Event

		Type	start
		At	Locator
		Part1	\POWERS\ElectricityControl\ChargedBolt01.part
		Part2	\POWERS\ElectricityControl\ChargedBolt02.part
		
		POther  Target
	End

End

Condition
	On 	Time
	Time 	67

	Event

		Type	start
		At	Locator
		Part1	\POWERS\ElectricityControl\ChargedBolt01.part
		Part2	\POWERS\ElectricityControl\ChargedBolt06.part
		Part3	\POWERS\ElectricityControl\ChargedBolt05.part
		Part4	\POWERS\ElectricityControl\ChargedBolt04.part
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
		EName 	Thingy
		Type	local
		At	Locator
		Part1	\POWERS\ElectricityControl\ElectricityControl01.part
		Part2	\POWERS\ElectricityControl\ElectricityControl02.part
		Part3	\POWERS\ElectricityControl\ElectricityInnerGlow.part
		Sound cagebolt2 80 80 .8
		
		

	End


	Event
		EName 	ThingyA
		Type	local
		At	Locator
		Part1	\POWERS\ElectricityControl\ElectricityOuterGlow.part
		Part2	\POWERS\ElectricityControl\ElectricityArch.part
		Part3	\POWERS\ElectricityControl\ElectricityRing.part
		
	End

End

Condition
	On 	Time
	Time 	25
	Event
		EName 	Burst
		Type	local
		At	Locator
		Part4	\POWERS\ElectricityControl\ElectricitySparkBurst.part
		SOund	lightningbolt2 80 30 .76
		LifeSpan	20

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
		
		EName	ThingyA
		Type	destroy
		
		
	End


End

End