#########################################################
##	CoT Red Crystal Damage Debuff
##
FxInfo

Input  
	InpName	Root
End

#####################################################################################


#####################  Eye Glows  ######################

Condition
	On 	Cycle
	Time 	15

	Event
		EName 	Prime
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.6 8.2 4.2
		End
		Part1	:PainFieldGlow2.part	
		LifeSpan	5
	End	

End

Condition
	On 	Cycle
	Time 	15

	Event
		EName 	Prime
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	-0.4 8.2 4.2
		End
		Part1	:PainFieldGlow2.part	
		LifeSpan	5 
	End	

End

Condition
	On 	Cycle
	Time 	20
	#Chance	1
	
	Event
		EName	Prime
		Type	Start
		At	root
		part1	:PainFieldAOERing.part
		part2	:PainFieldAOERing2.part
		part2	:PainFieldAOERing3.part
		part2	:PainFieldAOERing4.part
		Sound heal3b 60 60 .3
		LifeSpan	165
	End
	
	Event
		EName	Prime
		Type	Start
		At	root
		BhvrOverride
			PositionOffset 0 4 0
		End
		part3	:PainFieldSparkles.part
		part4	:PainFieldMist1.part
		LifeSpan	165
	End

End



End	


			