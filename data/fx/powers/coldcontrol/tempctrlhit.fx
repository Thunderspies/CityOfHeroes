#########################################################
##	
##
FxInfo

Input  
	InpName	Root
End

Input  
	InpName	HIPS
End

Input  
	InpName	CHEST
End

Input  
	InpName	HEAD
End

Input  
	InpName	wepR
End

Input  
	InpName	wepL
End

LifeSpan	200

##########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Root
		 part1	:bodychillbig.part
		 part2	:IceChunkzTempControl.part
	End
	
	Event
		Type	Local 
		At	Head
		part1   powers/coldcontrol/HeadLegschillbig.part
				
	End
	
	Event
		Type	Local 
		At	CHEST
	
		 part1	:bodychillbig.part
	End


	Event
		Type	Local 
		At	Hips
		
		part1	:bodychillbig.part
		part2   powers/coldcontrol/HeadLegschillbig.part
	End


End

######################################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	Local 
		At	Head

		part4	:TargetSnowfall.part
		
	End
	
	Event
		Type	Local 
		At	CHEST
		part4	:TargetSnowfall.part
		
	End


	Event
		Type	Local 
		At	Hips
		part4	:TargetSnowfall.part
		
	End


	Event
		Type	Local 
		At	WepL
		part1   :TargetSnowfall.part
		
	End

		Event
		Type	Local 
		At	WepR
		part1   :TargetSnowfall.part
		
	End



End

End			