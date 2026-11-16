#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	HandR
End

LifeSpan	100

############################################################################
##Hands
############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	:DrV_ElectricityControl01.part
		Part2	:DrV_ElectricityControl02.part
		Part3	:DrV_ElectricityInnerGlow.part
		Sound elecwindup_loop 80 80 .76
		
		

	End

	Event
		EName 	ThingyA
		Type	local
		At	HandR
		Part1	:DrV_ElectricityOuterGlow.part
		Part2	:DrV_ElectricityArch.part
		Part3	:DrV_ElectricityRing.part
		
	End
	
End

End