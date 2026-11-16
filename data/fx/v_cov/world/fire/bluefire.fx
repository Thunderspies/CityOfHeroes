#########################################################
##	
##
FxInfo

Flags	IsArmor

#####################################################################################
##Sound
#####################################################


Condition
		
	Event
		Type	Local
		At 	origin
		Sound Mineral_loop 70 70 .72
		Flags	PowerLoopingSound
		
	End

#############################################################

	Event
	
		Type	start
		At	head
		
		Part	:PlasmaHandz.part		
		Part	:PlasmaHandz2.part
		Part	:Embers.part
				
	End
		
End


End
