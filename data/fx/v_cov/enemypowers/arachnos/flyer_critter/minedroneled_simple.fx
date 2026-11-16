#########################################################
##	
##

FxInfo

##################################


Condition
	On 	Time
	Time 	0

	Event
		
		Ename	CoreNodez
		Type	local
		At 	Hair	
		BhvrOverride
			PyrRotate	0 0 0					
			PositionOffset	0 -.07 0
			Spin		 0 .2 0
		End
		Geom	Led_Offset
		
	End
	
	Event
		
		Type	local
		At 	CoreNodez
		altpiv	1
		Part	:Red_LED.part

		
	End


End

End