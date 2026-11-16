#########################################################
##	Fire_Ball
##
FxInfo

Input  
	InpName	root
End


Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	root
		
		part1	World/Lairs/cot/GreenPortal.part
		#part2	World/Lairs/cot/PortalRings.part
		Sound Magic8_loop 48 48 .9
	End

		
	
End

End	


			