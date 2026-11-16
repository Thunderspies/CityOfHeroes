#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	50


#Condition
#	On	Time
#	Time	0
#
#	Event
#		Type Local
#		At T_Root
#		Sound	Sonic3_loop 100 80 .22
#	End
#End
#

Condition
	On 	Time
	Time 	15

	Event
			
		Type	local
		At	T_Root
		
		Part	:SonicRing.part
		Part	:SonicRingCore.part
			
		LifeSpan	21

	End

End

End	


			