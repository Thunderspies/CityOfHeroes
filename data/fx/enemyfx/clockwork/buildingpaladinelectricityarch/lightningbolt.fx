#########################################################
##	forceBubble
########################################################
FxInfo

LifeSpan	200

########################################################


Condition
	On 	Time
	Time 	25

	Event
		Type	start
		At	T_chest
		Part2	:ElectricityArchSmall.part
		Part2	:ElectricityArchSmall1.part
		lifespan	42	
	End

	Event
		Type	start
		At	T_chest
		Part1	:ElectricityBodyGlow.part
		Part1	:ElectricityBodyGlowstar.part
		Part	:BlueSpark.part
		Magnet	T_chest
		
		lifespan	33	
	End

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
	Time 	33

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
	Time 	36

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
	Time 	40

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
	Time 	43

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
		SOund	PaladinDraw2 80 30 .88
		

	End

	
End


End