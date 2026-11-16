#########################################################
##	Fire_Ball
##
FxInfo

Input  
	InpName	Origin
End

Condition
	On	Time
	Time	0

	Event	
		Type	Local
		At	origin
		Sound portal4_loop 60 60 .48
	End
End


Condition
	On	Time
	Time	0

	Event	
		Type	Local
		At	origin
		Sound whispers2_loop 60 60 .48
	End
End


Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Origin
		
		BhvrOverride
			PositionOffset	0 0 1
		End


		part1	World/Lairs/cot/LargePortal.part
		#part2	World/Lairs/cot/PortalRings.part
	End

		
	
End

End	


			