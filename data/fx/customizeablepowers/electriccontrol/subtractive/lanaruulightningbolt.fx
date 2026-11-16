#########################################################
##	forceBubble
########################################################
FxInfo


LifeSpan	115    

########################################################


######################################################################################

Condition
	On 	Time
	Time 	18

	Event
		Type	local
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
	Time 	25

	Event
		Type	local
		At	HandL
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	28

	Event
		Type	local
		At	HandL
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		Part	:ChargedBolt01.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	32

	Event
		Type	local
		At	HandL
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		Part	:ChargedBolt06.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	35

	Event
		Type	local
		At	HandL
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	39

	Event
		Type	local
		At	HandL
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		Part	:ChargedBolt06.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	42

	Event
		Type	local
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
		Type	local
		At	HandL
		
		Part1	:ChargedBolt06.part
		Part2	:ChargedBolt05.part
		Part	:ChargedBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	49
		
	Event
		Type	local
		At	HandL
		
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part
		
		POther  Target
	End
End

Condition
	On 	Time
	Time 	52
		
	Event
		Type	local
		At	HandL
		
		Part1	:ChargedBolt03.part
		Part2	:ChargedBolt04.part
		
		POther  Target
	End
End

Condition
	On 	Time
	Time 	56
		
	Event
		Type	local
		At	HandL
		
		Part1	:ChargedBolt01.part
		Part2	:ChargedBolt02.part
		
		POther  Target
	End

End

Condition
	On 	Time
	Time 	60
		
	Event
		Type	local
		At	HandL
		
		Part1	:ChargedBolt06.part
		Part2	:ChargedBolt05.part
		Part3	:ChargedBolt01.part
		Part4	:ChargedBolt02.part	
		POther  Target
	End

End

Condition
	On 	Time
	Time 	64

	Event
		Type	local
		At	HandL
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	67

	Event
		Type	local
		At	HandL
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	71

	Event
		Type	local
		At	HandL
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		Part	:ChargedBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	75

	Event
		Type	local
		At	HandL
		Part	:ChargedBolt06.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	79
		
	Event
		Type	local
		At	HandL
		
		Part	:ChargedBolt06.part
		Part	:ChargedBolt05.part
		
		POther  Target
	End
End

Condition
	On 	Time
	Time 	83
		
	Event
		Type	local
		At	HandL
		
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		
		POther  Target
	End
End

Condition
	On 	Time
	Time 	87
		
	Event
		Type	local
		At	HandL
		
		Part	:ChargedBolt03.part
		Part	:ChargedBolt04.part
		
		POther  Target
	End
End

Condition
	On 	Time
	Time 	91

	Event
		Type	local
		At	HandL
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt04.part
		Part	:ChargedBolt06.part
		Part	:ChargedBolt05.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	95

	Event
		Type	local
		At	HandL
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	99

	Event
		Type	local
		At	HandL
		Part	:ChargedBolt03.part
		Part	:ChargedBolt04.part
		Part	:ChargedBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	103
		
	Event
		Type	local
		At	HandL
		Part	:ChargedBolt03.part
		Part	:ChargedBolt06.part
		Part	:ChargedBolt05.part
		
		POther  Target
	End
End

Condition
	On 	Time
	Time 	107
		
	Event
		Type	local
		At	HandL
		
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		
		POther  Target
	End
End

Condition
	On 	Time
	Time 	111
		
	Event
		Type	local
		At	HandL
		
		Part	:ChargedBolt06.part
		Part	:ChargedBolt05.part
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		
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
		EName 	Thingy1
		Type	local
		At	HandL
		Part	:ElectricityControl01.part
		Part	:ElectricityControl02.part
		Part	:ElectricityInnerGlow.part
		Part	:ElectricityOuterGlow.part
		Part	:ElectricityArch.part
		Part	:ElectricityRing.part
		Sound chargebolt 100 100 .77
		
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName 	BurstL
		Type	local
		At	HandL
		Part4	:ElectricitySparkBurst.part
		LifeSpan	20

	End
End


End