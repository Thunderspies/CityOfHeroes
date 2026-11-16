#########################################################
##	forceBubble
########################################################
FxInfo



LifeSpan	200

########################################################


######################################################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	start
		At	WepL
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
		At	WepL
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
		At	WepL
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
		At	WepL
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
		At	WepL
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
		At	WepL
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
		At	WepL
		#Part2	:QuickBolt01a.part
		Part3	:ChargedBolt06.part
		Part4	:ChargedBolt05.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

### more  ######


Condition
	On 	Time
	Time 	49

	Event
		Type	start
		At	WepL
		Part1	:ChargedBolt06.part
		Part2	:ChargedBolt05.part
		Part3	:ChargedBolt01.part
		Part4	:ChargedBolt02.part
		POther Target	
	End		
	
End

Condition
	On 	Time
	Time 	52

	Event
		Type	start
		At	WepL
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	55

	Event
		Type	start
		At	WepL
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	59

	Event
		Type	start
		At	WepL
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	63

	Event
		Type	start
		At	WepL
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	67

	Event
		Type	start
		At	WepL
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	71
		
	Event

		Type	start
		At	WepL
		#Part2	:QuickBolt01a.part
		Part3	:ChargedBolt06.part
		Part4	:ChargedBolt05.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	80

	Event
		Type	start
		At	WepL
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	85

	Event
		Type	start
		At	WepL
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	90

	Event
		Type	start
		At	WepL
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
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
		EName 	Thingy1
		Type	local
		At	WepL
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		
	End




	Event
		EName 	ThingyB
		Type	local
		At	WepL
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		
	End
End

Condition
	On 	Time
	Time 	25


	Event
		EName 	BurstL
		Type	local
		At	WepL
		Part4	:ElectricitySparkBurst.part
		LifeSpan	20

	End

End



Condition
	On 	Time
	Time 	95


	Event
		
		EName	Thingy1
		Type	destroy
		
		
	End



	Event
		
		EName	ThingyB
		Type	destroy
		
		
	End

	

End

End