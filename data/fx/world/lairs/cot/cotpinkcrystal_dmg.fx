#########################################################
##	CoT Pink Crystal Damage Debuff
##
FxInfo

Input  
	InpName	Root
End

#####################################################################################



Condition
	
	Event
		EName	crystal
		Type	Local
		At	root
		Geom	_cot_spcl_crystal_Pink
		Sound cotcrystbad_loop 30 30 .6
	End

End

#Condition
#	On 	Cycle
#	Time 	20
#	#Chance	1
#	
#	Event
#		EName	Prime
#		Type	Local
#		At	root
#		part1	:CotPinkCrystalAOERing.part
#		part2	:CotPinkCrystalAOERing2.part
#		part3	:CotPinkCrystalSparkles.part
#		part4	:CotPinkCrystalMist1.part
#		Sound	heal3b 60 20 .3
#		LifeSpan	165
#	End
#
#End



End	


			