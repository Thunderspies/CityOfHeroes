#########################################################
##	
##

FxInfo
LifeSpan  150

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

##################################


Condition
	On 	Time
	Time 	0
	Event
		ename	STARTER
		Type	local 
		At	origin
	End

	
	Event
		ename	F01
		Type	local 
		At	STARTER
		#geom    Testing_Target		
		bhvr	:BS_STing_tendrildir.bhvr
		lifespan 50
		BhvrOverride
			scale 1.5 1.5 1.5
		End

	End
	Event
		ename	F02
		Type	local 
		At	STARTER
		#geom    Testing_Target		
		bhvr	:BS_STing_tendrildir.bhvr
		lifespan 50
		BhvrOverride
			scale 1.6 1.6 1.6
		End

	End
	Event
		ename	F03
		Type	local 
		At	STARTER
		#geom    Testing_Target		
		bhvr	:BS_STing_tendrildir.bhvr
		lifespan 50
		BhvrOverride
			scale 1 1 1
		End
	End
	Event
		ename	F04
		Type	local 
		At	STARTER
		#geom    Testing_Target		
		bhvr	:BS_STing_tendrildir.bhvr
		lifespan 50		
		BhvrOverride
			scale 1.1 1.1 1.1
		End
	End
	Event
		ename	F05
		Type	local 
		At	STARTER
		#geom    Testing_Target		
		bhvr	:BS_STing_tendrildir.bhvr
		lifespan 50		
		BhvrOverride
			scale 1.2 1.2 1.2
		End
	End
	
end

Condition
	On 	Time
	Time 	2


	Event
		Type	start 
		At	F01
		Part  :BS_Sting_Tendril.part
		Part  :BS_Sting_TendrilSharp.part
		lifespan 6
	End
	Event
		Type	start 
		At	F02
		Part  :BS_Sting_Tendril.part
		Part  :BS_Sting_TendrilSharp.part
		lifespan 6	
	End
end



Condition
	On 	Time
	Time 	3

	Event
		Type	start 
		At	F03
		Part  :BS_Sting_Tendril.part
		Part  :BS_Sting_TendrilSharp.part	
		lifespan 6
	End
	Event
		Type	start 
		At	F04
		Part  :BS_Sting_Tendril.part
		Part  :BS_Sting_TendrilSharp.part	
		lifespan 6
	End
	Event
		Type	start 
		At	F05
		Part  :BS_Sting_Tendril.part
		Part  :BS_Sting_TendrilSharp.part	
		lifespan 6
	End


end

			