#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	HandR
End

##LifeSpan	100   ## (no lifespan for windup -steve)

############################################################################
##Hands
############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Sound elecwindup_loop 80 80 .5
		

	End

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	:Red_ElectricityControl01.part
		Part2	:Red_ElectricityControl02.part
		Part3	:Red_ElectricityInnerGlow.part
		Part1	:Red_ElectricityControl01white.part
		Part2	:Red_ElectricityControl02white.part
		
		BhvrOverride
			FadeOutLength		200
		End

		

	End

	Event
		EName 	ThingyA
		Type	local
		At	HandR
		Part1	:Red_ElectricityControl01.part
		Part2	:Red_ElectricityControl02.part
		Part3	:Red_ElectricityInnerGlow.part
		Part1	:Red_ElectricityControl01white.part
		Part2	:Red_ElectricityControl02white.part
		
		BhvrOverride
			FadeOutLength		200
		End

	End
	
End

End